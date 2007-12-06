#include "MainWindow.h"
#include "PostData.h"
#include "AboutDlg.h"
#include "ConfigDlg.h"
#include "palette.h"
#include "ParamsDlg.h"

#include <cassert>
#include <QFile>
#include <QBuffer>
#include <QMessageBox>
#include <QCloseEvent>
#include <QHeaderView>
#include <QTcpSocket>
#include <QInputDialog>
#include <QtDebug>
#include <openssl/sha.h>
#include <openssl/blowfish.h>
#include <iostream>

#ifndef _WIN32
#	include <net/if.h>
#	include <sys/ioctl.h>
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#else
#	include <windows.h>
#	include <winsock2.h>
#	include <lm.h>
#endif

#define PROTOCOL_VERSION "1.02"
#define CLIENT_VERSION "0.9.95"

MainWindow::MainWindow(QString infile)
	: m_nPing(0), m_nCycle(0), m_pChat(0), m_nGame(-1), m_state(StateConnecting), m_bWaitForRoom(false)
{
	m_ping = new QUdpSocket(this);
	
	prepareGUI();
	connect(&m_http, SIGNAL(requestFinished(int,bool)), this, SLOT(requestFinished(int,bool)));
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
	connect(actionAbout, SIGNAL(triggered()), this, SLOT(aboutDialog()));
	connect(actionConfig, SIGNAL(triggered()), this, SLOT(configDialog()));
	connect(actionWaitForRoom, SIGNAL(triggered()), this, SLOT(toggleWaitForRoom()));
	connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(m_ping, SIGNAL(readyRead()), this, SLOT(readPing()));
	connect(treeGames, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(gameDoubleClicked(const QModelIndex&)));
	connect(pushBack, SIGNAL(clicked()), this, SLOT(switchBack()));
	connect(pushStart, SIGNAL(clicked()), this, SLOT(start()));
	connect(pushChat, SIGNAL(clicked()), this, SLOT(chatSend()));
	connect(&m_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
	connect(&m_process, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(processStateChanged(QProcess::ProcessState)));
	connect(pushParameters, SIGNAL(clicked()), this, SLOT(showParameters()));
	
	m_ping->bind();
	
	// nacist zdrojovy soubor
	QFile file (infile);
	if(!file.open(QIODevice::ReadOnly))
	{
		throw QString::fromUtf8("Nelze otevřít vstupní soubor!");
	}
	else
	{
		parseKeyValue(&file, m_mapInput);
		file.close();
	}
	
	if(!m_mapInput.contains("Params"))
		throw QString::fromUtf8("Vstupní soubor je neplatný.");
	
	// desifrovat data ze vstupu
	m_array = decryptString(m_mapInput.value("Params"));
	QBuffer buffer(&m_array);
	buffer.open(QBuffer::ReadOnly);
	parseKeyValue(&buffer, m_mapInput);
	
	dumpKeyValue(m_mapInput);
	
	m_codec = QTextCodec::codecForName("Windows-1250");
	
	//stahnout config
	printStatus(QString::fromUtf8("Stahuji herní konfiguraci..."));
	sendGetQuery(QUrl("http://gpclient.dolezel.info:80/client/gameconfig.php"), RequestGameConfig);
	
	m_timer.start(10000);
}

MainWindow::~MainWindow()
{
	delete m_statusLabel;
	delete m_pChat;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	if(terminateGame())
	{
		if(m_state != StateExiting)
		{
			m_timer.stop();
			sendLogout();
			event->ignore();
		}
	}
}

void MainWindow::aboutDialog()
{
	AboutDlg(this).exec();
}

void MainWindow::prepareGUI()
{
	setupUi(this);
	
	QList<int> sizes;
	sizes.append(150);
	sizes.append(400);
	splitterVert->setSizes(sizes);
	
	sizes.clear();
	sizes.append(300);
	sizes.append(80);
	splitterHoriz->setSizes(sizes);
	
	lineChat->setFocus(Qt::OtherFocusReason);
	
	QStringList headerData;
	headerData << QString::fromUtf8("Jméno") << "Ping" << "ID hry";
	
	m_pUsersModel = new UserTreeModel(headerData);
	m_pUsersModel->setRowHeight(25);
	treeUsers->setModel(m_pUsersModel);
	
	treeUsers->header()->resizeSection(0, 220);
	treeUsers->header()->resizeSection(1, 50);
	treeUsers->header()->resizeSection(2, 50);
	
	m_pUsersModel->rootItem->appendChild(new TreeItem(QString::fromUtf8("Přihlížející uživatelé"), m_pUsersModel->rootItem));
	m_pUsersModel->rootItem->appendChild(new TreeItem(QString::fromUtf8("Hrající uživatelé"), m_pUsersModel->rootItem));
	m_pUsersModel->rootItem->appendChild(new TreeItem(QString::fromUtf8("Čekající uživatelé"), m_pUsersModel->rootItem));
	
	for(int i=0;i<3;i++)
		treeUsers->expand( m_pUsersModel->index(i, 0, QModelIndex()) );
	
	headerData.clear();
	headerData << "ID" << QString::fromUtf8("Jméno") << /*"Hra" <<*/ "Stav" << QString::fromUtf8("Délka hry") << QString::fromUtf8("Hráči");
	
	m_pGamesModel = new GameTreeModel(headerData);
	m_pGamesModel->setRowHeight(25);
	treeGames->setModel(m_pGamesModel);
	
	treeGames->header()->resizeSection(0, 25);
	treeGames->header()->resizeSection(1, 400);
	//treeGames->header()->resizeSection(2, 200);
	
	headerData.clear();
	headerData << QString::fromUtf8("Jméno hráče") << "Ping" << QString::fromUtf8("Může měnit parametry/spustit hru");
	m_pServerModel = new TreeModel(headerData);
	m_pServerModel->setRowHeight(25);
	treeServerInfo->setModel(m_pServerModel);
	treeServerInfo->header()->resizeSection(0, 400);
	
	headerData.clear();
	headerData << QString::fromUtf8("Jméno hráče") << "Ping" << QString::fromUtf8("Skóre") << QString::fromUtf8("Čas");
	m_pGameServerModel = new TreeModel(headerData);
	m_pGameServerModel->setRowHeight(25);
	treeGameInfo->setModel(m_pGameServerModel);
	treeGameInfo->header()->resizeSection(0, 400);
	
	m_statusLabel = new QLabel(statusbar);
	statusbar->addWidget(m_statusLabel);
	m_statusLabel->setText(QString::fromUtf8("<font color=\"yellow\">Získávám informace o aktualizacích</font>"));
	
	// styl
	applyPalette(this, PaletteMain);
	applyPalette(toolBar, PaletteToolbar);
	applyPalette(treeUsers, PaletteTree);
	applyPalette(treeGames, PaletteTree);
	applyPalette(treeServerInfo, PaletteTree);
	applyPalette(treeGameInfo, PaletteTree);
	applyPalette(lineChat, PaletteEdit);
	applyPalette(textChat, PaletteEdit);
}

void MainWindow::update()
{
	if(m_ping->hasPendingDatagrams())
		qDebug() << "ERROR: Unprocessed datagrams";
	// poslat UDP ping
	m_ping->write("\xff\xff\xff\xffping", 9);
	m_pingTime.start();
	
	if(m_state != StateConnecting && m_state != StateConnectingWait)
	{
		sendUserList();
		if(m_state == StateRooms)
		{
			actionWaitForRoom->setEnabled(true);
			sendGameList();
		}
		else if(m_state == StateGame)
		{
			actionWaitForRoom->setEnabled(false);
			if(m_process.state() != QProcess::NotRunning)
			{
				if(++m_nCycle < 3)
					return;
			}
			sendGameAll(AllNone);
		}
	}
}

void MainWindow::readPing()
{
	char buf[100];
	qDebug() << "Ping response received";
	
	m_ping->readDatagram(buf, 100);
	m_nPing = m_pingTime.elapsed();
}

void MainWindow::parseKeyValue(QIODevice* pSrc, QMap<QString,QString>& out)
{
	assert(pSrc != NULL);
	QTextStream stream(pSrc);
	
	while(!stream.atEnd())
	{
		QString key, value;
		QString line = stream.readLine().trimmed();
		int eq = line.indexOf('=');
		
		if(eq == -1)
			continue;
		
		out[line.left(eq)] = line.mid(eq+1);
	}
}

void MainWindow::dumpKeyValue(const QMap<QString,QString>& map)
{
	QMapIterator<QString,QString> it(map);
	
	while(it.hasNext())
	{
		it.next();
		qDebug() << it.key() << "=" << it.value();
	}
}

QByteArray MainWindow::decrypt(const QByteArray& data)
{
	SHA_CTX ctx;
	BF_KEY key;
	unsigned char ivec[8];
	char hash[SHA_DIGEST_LENGTH];
	QByteArray out;
	
	out.resize(data.size());

	// vygenerovat key
	SHA1_Init(&ctx);
	SHA1_Final((unsigned char*) hash, &ctx);
	BF_set_key(&key, sizeof(hash), (unsigned char*) hash);

	// vygenerovat ivec
	memset(ivec,0,sizeof(ivec));
	BF_ecb_encrypt(ivec,ivec,&key,BF_ENCRYPT);

	// desifrovat
	for(int i=0;i<data.length();i++)
	{
		unsigned char dec[8];
		unsigned char tb = data[i];

		BF_ecb_encrypt(ivec,dec,&key,BF_ENCRYPT);
		out[i] = data[i] ^ dec[0];
		memmove(ivec,&ivec[1],8-1);
		ivec[7] = tb;
	}
	
	return out;
}

QByteArray MainWindow::encrypt(const QByteArray& data)
{
	SHA_CTX ctx;
	BF_KEY key;
	unsigned char ivec[8];
	char hash[SHA_DIGEST_LENGTH];
	QByteArray out;
	
	out.resize(data.size());

	// vygenerovat key
	SHA1_Init(&ctx);
	SHA1_Final((unsigned char*) hash, &ctx);
	BF_set_key(&key, sizeof(hash), (unsigned char*) hash);

	// vygenerovat ivec
	memset(ivec,0,sizeof(ivec));
	BF_ecb_encrypt(ivec,ivec,&key,BF_ENCRYPT);

	// desifrovat
	for(int i=0;i<data.length();i++)
	{
		unsigned char dec[8];

		BF_ecb_encrypt(ivec,dec,&key,BF_ENCRYPT);
		out[i] = data[i] ^ dec[0];
		memmove(ivec,&ivec[1],8-1);
		ivec[7] = out[i];
	}
	
	return out;
}

QByteArray MainWindow::decryptString(const QString& data)
{
	return decrypt(QByteArray::fromBase64(data.toUtf8()));
}

QString MainWindow::encryptString(const QByteArray& data)
{
	return encrypt(data).toBase64();
}

#ifndef _WIN32

QString MainWindow::generateLocalIP()
{
	QProcess ifc;
	QString mac,ip;
	
	if(!getIPMac("ppp0",ip,mac))
	{
		if(!getIPMac("wlan0",ip,mac))
			getIPMac("eth0",ip,mac);
	}
	
	if(!mac.isEmpty())
		mac += ',';
	if(ip.isEmpty())
		ip = QString("192.168.0.%1").arg(rand());
	mac += ip;
		
	return QString("LocalIP=%1,mb:None,sg:3577009%2,sg:29154625%3,pm:3030303030303030303030303030303030303130,pm:3130303030303030303030303030303030303130,cp:0FEBFBFF00000F%4,\r\n").arg(mac).arg(rand(),2).arg(rand(),2).arg(rand(),2);
	//}
}

bool MainWindow::getIPMac(QString iface, QString& ip, QString& mac)
{
	int fd = socket(AF_INET,SOCK_DGRAM, 0);
	struct ifreq ifr;
	
	ifr.ifr_addr.sa_family = AF_INET;
	strcpy(ifr.ifr_name, iface.toLatin1());
	
	if(ioctl(fd, SIOCGIFADDR, &ifr))
	{
		::close(fd);
		return false;
	}
	else
	{
		mac = "%1-%2-%3-%4-%5-%6";
		u_char* addr = (u_char*)&(((struct sockaddr_in * )&ifr.ifr_addr)->sin_addr);
		ip = QString("%1.%2.%3.%4").arg(int(addr[0])).arg(int(addr[1])).arg(int(addr[2])).arg(int(addr[3]));
		
		ifr.ifr_addr.sa_family = AF_INET;
		strcpy(ifr.ifr_name, iface.toLatin1());
		
		if(ioctl(fd, SIOCGIFHWADDR, &ifr))
		{
			mac = "00-00-00-00-00-00";
			::close(fd);
			return false;
		}
		
		::close(fd);
		for(int i=0;i<6;i++)
		{
			mac = mac.arg((unsigned char)(ifr.ifr_hwaddr.sa_data[i]), 2, 16, QChar('0'));
		}
		
		return true;
	}
}

#else

QString MainWindow::generateLocalIP()
{
	unsigned char MACData[8];
	WKSTA_TRANSPORT_INFO_0 *pwkti;
	DWORD dwEntriesRead;
	DWORD dwTotalEntries;
	BYTE *pbBuffer;

	NET_API_STATUS dwStatus = NetWkstaTransportEnum(0,0,&pbBuffer,MAX_PREFERRED_LENGTH,&dwEntriesRead,&dwTotalEntries,0);
	if(dwStatus == NERR_Success)
	{
		pwkti = (WKSTA_TRANSPORT_INFO_0 *)pbBuffer; // type cast the buffer
		for(DWORD i=1;i<dwEntriesRead;i++)
		{
			swscanf((_TCHAR*)pwkti[i].wkti0_transport_address,
				L"%2hx%2hx%2hx%2hx%2hx%2hx",&MACData[0],&MACData[1],&MACData[2],
				&MACData[3],&MACData[4],&MACData[5]);
			break;
		}
	}
	NetApiBufferFree(pbBuffer);
	
	char name[255];
	PHOSTENT hostinfo;
	
	gethostname(name,sizeof(name));
	hostinfo = gethostbyname(name);
	
	QString mac = "%1-%2-%3-%4-%5-%6";
	for(int i=0;i<6;i++)
		mac = mac.arg(MACData[i], 2, 16, QChar('0'));
	
	mac += ',';
	if(hostinfo->h_addr_list[0])
		mac += inet_ntoa(*(struct in_addr *)hostinfo->h_addr_list[0]);
	else
		mac += QString("192.168.0.%1").arg(rand());
	
	QMessageBox::critical(0, "GamePark klient",mac);
	
	return QString("LocalIP=%1,mb:None,sg:3577009%2,sg:29154625%3,pm:3030303030303030303030303030303030303130,pm:3130303030303030303030303030303030303130,cp:0FEBFBFF00000F%4,\r\n").arg(mac).arg(rand(),2).arg(rand(),2).arg(rand(),2);
}

#endif

void MainWindow::requestFinished(int id, bool error)
{
	if(error)
		qDebug() << "Error:" << m_http.errorString();
	
	if(!m_mapRequests.contains(id))
		;//qCritical("requestFinished(): Unknown request id %d", id);
	else
	{
		QMap<int,Request>::iterator it = m_mapRequests.find(id);
		Request request = it.value();
		
		qDebug() << "requestFinished:" << id << "; error:" << error;
		
		if(error)
			printError(QString("Chyba socketu: ") + m_http.errorString());
		else
			request.buffer->seek(0);
		
		m_mapRequests.erase(it);
		
		switch(request.type)
		{
		case RequestGameConfig:
			doneGameConfig(*request.buffer, error); break;
		case RequestGetVersion:
			doneGetVersion(*request.buffer, error); break;
		case RequestAuth:
			doneAuth(*request.buffer, error); break;
		case RequestConnect:
			doneConnect(*request.buffer, error); break;
		case RequestLogin:
			doneLogin(*request.buffer, error); break;
		case RequestParamsMaps:
			doneParamsMaps(*request.buffer, error); break;
		case RequestLogout:
			doneLogout(*request.buffer, error); break;
		case RequestUserList:
			doneUserList(*request.buffer, error); break;
		case RequestGameList:
			doneGameList(*request.buffer, error); break;
		case RequestSetGame:
			doneSetGame(*request.buffer, error); break;
		case RequestGameAll:
			doneGameAll(*request.buffer, error); break;
		}
		
		delete request.buffer;
	}
}

void MainWindow::doneGameConfig(QBuffer& buffer, bool error)
{
	if(error)
	{
		printError(QString::fromUtf8("Nepodařilo se stáhnout konfigurační data, bez nich nelze spouštět hry"));
	}
	else
	{
		m_strSkin = processGameConfig(buffer);
		actionConfig->setEnabled(true);
		sendAuth(m_array);
		
		/*if(m_state == StateConnectingWait)
		{
			m_state = StateConnecting;
			qDebug() << "Resuming delayed connect";
			sendLogin();
		}*/
		
		// ziskat info o updatu
		sendGetQuery(QUrl("http://gpclient.dolezel.info:80/client/getversion.php?myver=" CLIENT_VERSION), RequestGetVersion);
	}
}

void MainWindow::doneGetVersion(QBuffer& buffer, bool error)
{
	if(error)
	{
		m_statusLabel->setText(QString::fromUtf8("<font color=\"yellow\">Nepodařilo se získat informace o aktualizacích</font>"));
	}
	else
	{
		if(buffer.data() == CLIENT_VERSION)
			m_statusLabel->setText(QString::fromUtf8("<font color=\"green\">Máte nejaktuálnější verzi klienta</font>"));
		else
			m_statusLabel->setText(QString::fromUtf8("<font color=\"red\">Vyšla nová verze klienta: %1</font>").arg(buffer.data().constData()));
	}
}

void MainWindow::sendPostQuery(QUrl url, QByteArray postdata, RequestType type)
{
	QHttpRequestHeader header;
	Request request = { type, new QBuffer };
	int id;
	
	request.buffer->open(QBuffer::ReadWrite);
	
	m_http.setSocket(new QTcpSocket);
	m_http.setHost(url.host(), url.port());
	header.setRequest("POST", url.path());
	header.addValue("User-Agent", "Mozilla/4.0 (compatible; Synapse)");
	header.addValue("Content-Type", "application/x-www-form-urlencoded");
	header.addValue("Host", url.host() + ":" + QString::number(url.port()));
	header.addValue("Connection", "close");
	
	id = m_http.request(header, postdata, request.buffer);
	m_mapRequests[id] = request;
	
	qDebug() << "Queued request id " << id << "to" << url;
}

void MainWindow::sendGetQuery(QUrl url, RequestType type)
{
	Request request = { type, new QBuffer };
	int id;
	
	request.buffer->open(QBuffer::ReadWrite);
	
	m_http.setSocket(new QTcpSocket);
	m_http.setHost(url.host(), url.port());
	id = m_http.get(url.path(), request.buffer);
	m_mapRequests[id] = request;
}

void MainWindow::sendAuth(QByteArray encdata)
{
	PostData fields;
	QString localip = generateLocalIP();
	
	encdata.append(localip);
	encdata.append(QString("SkinGps=%1\r\n").arg(m_strSkin));
	
	fields.set("Params", QUrl::toPercentEncoding(encryptString(encdata)));
	
	printStatus(QString::fromUtf8("Odesílám autorizaci"));
	sendPostQuery(m_mapInput.find("AuthURL").value(), fields.str(), RequestAuth);
}

void MainWindow::doneAuth(QBuffer& buffer, bool error)
{
	if(error || buffer.data().startsWith("ERROR"))
		printError("Autorizace selhala!");
	else
	{
		m_strAuth = buffer.data();
		update();
		sendConnect();
	}
}

void MainWindow::sendConnect()
{
	m_strID = QString::number( size_t(rand()) + size_t(rand()) );
	
	m_url.setScheme("http");
	m_url.setHost(m_mapInput.value("IP"));
	m_url.setPort(m_mapInput.value("Port").toUShort());
	m_url.setPath("/cgi-bin/gameserver.cgi");
	m_ping->connectToHost(m_url.host(), m_url.port()+1);
	
	qDebug() << m_url;
	
	PostData fields;
	fields.set("$FORMAT$", "ServerConnect");
	fields.set("$VER$", PROTOCOL_VERSION);
	fields.set("PLAYER", "");
	fields.set("ID", m_strID);
	fields.set("T", QString::number(m_nPing));
	
	printStatus(QString::fromUtf8("Odesílám ServerConnect..."));
	sendPostQuery(m_url, fields.str(), RequestConnect);
}

void MainWindow::doneConnect(QBuffer& buffer, bool error)
{
	parseKeyValue(&buffer, m_mapConnect);
	
	if(error || m_mapConnect.value("$FORMAT$") == "ErrorMessage")
		printError("ServerConnect selhal!");
	else
	{
		updateGeneralInfo();
		sendLogin();
	}
}

void MainWindow::updateGeneralInfo()
{
	labelGeneralInfo->setText( QString::fromUtf8("<b>%1</b> (%2) jako hráč <i>%3</i>")
			.arg(m_mapConnect.value("GAME")).arg(m_mapConnect.value("ROOM")).arg(m_mapLogin.value("NAME")));
}

void MainWindow::sendLogin()
{
	qDebug() << "*** sendLogin()";
	if(haveGameConfig())
	{
		if(!haveGameConfig(m_mapConnect.value("TYPE"))) // zjistit, jestli je hra podporovana
		{	
			printError(QString::fromUtf8("Hra \"%1\" není klientem podporována").arg(m_mapConnect.value("TYPE")));
			printError(QString::fromUtf8("Pokud jste si jist, že hra na vaší platformě funguje, požádejte o její přidání."));
			
			/*QMessageBox::critical(this, "GamePark klient",
			QString::fromUtf8("Tato hra není klientem podporována. Pokud jste si jist, že hra na vaší platformě funguje, požádejte o její přidání."));*/
		}
		else
		{
			PostData fields;
			fields.set("$FORMAT$", "ServerLogin3");
			fields.set("$VER$", PROTOCOL_VERSION);
			fields.set("PLAYER", "");
			fields.set("ID", m_strID);
			fields.set("T", QString::number(m_nPing));
			fields.set("Params", m_strAuth);
			fields.set("ID", m_strID);
	
			printStatus(QString::fromUtf8("Přihlašuji se"));
			sendPostQuery(m_url, fields.str(), RequestLogin);
		}
	}
	else
	{
		// musi se pockat na stazeni informaci
		m_state = StateConnectingWait;
	}
}

void MainWindow::doneLogin(QBuffer& buffer, bool error)
{
	parseKeyValue(&buffer, m_mapLogin);
	
	if(error || m_mapLogin.value("$FORMAT$") == "ErrorMessage")
		printError(QString::fromUtf8("Přihlášení selhalo!"));
	else
	{
		updateGeneralInfo();
		sendParamsMaps();
		
		printStatus(QString::fromUtf8("Připojuji se k chatu..."));
		m_pChat = new GPChat(m_mapInput.value("ChatIP"), m_mapInput.value("ChatPort").toUShort(),
				     m_mapLogin.value("NAME"), m_mapInput.value("Pokoj").toLong(), m_mapInput.value("ChatPassword"));
		
		connect(m_pChat, SIGNAL(errorMessage(QString)), this, SLOT(printError(QString)));
		connect(m_pChat, SIGNAL(userMessage(QString,QString)), this, SLOT(userMessage(QString,QString)));
		connect(m_pChat, SIGNAL(userJoin(QString)), this, SLOT(userJoin(QString)));
		connect(m_pChat, SIGNAL(statusMessage(QString)), this, SLOT(printStatus(QString)));
		connect(m_pChat, SIGNAL(userLeave(QString,QString)), this, SLOT(userLeave(QString,QString)));
	}
}

void MainWindow::sendParamsMaps()
{
	PostData fields;
	fields.set("$FORMAT$", "GameParamsMaps");
	fields.set("$VER$", PROTOCOL_VERSION);
	fields.set("PLAYER", "");
	fields.set("ID", m_strID);
	fields.set("T", QString::number(m_nPing));
	fields.set("GAME", "0");
	
	printStatus(QString::fromUtf8("Stahuji informace o mapách..."));
	sendPostQuery(m_url, fields.str(), RequestParamsMaps);
}

void MainWindow::doneParamsMaps(QBuffer& buffer, bool error)
{
	if(error || buffer.data().startsWith("$FORMAT$=ErrorMessage"))
		printError(QString::fromUtf8("Stažení informací o mapách selhalo!"));
	else
	{
		QTextStream stream(&buffer);
		QString line;
		
		// seznam map
		while(!stream.atEnd())
		{
			line = stream.readLine().trimmed();
			
			if(line[0] == '$')
				continue;
			
			long maps = line.toLong();
			qDebug() << "Loading " << maps << " maps";
			
			while(maps-- && !stream.atEnd())
			{
				m_listMaps.append( stream.readLine().trimmed().remove('"') );
			}
			break;
		}
		
		// seznam parametru
		while(!stream.atEnd())
		{
			line = stream.readLine().trimmed();
			
			long params = line.toLong();
			qDebug() << "Loading " << params << " params";
			
			while(params-- && !stream.atEnd())
			{
				int lastq;
				QStringList parinfo;
				
				line = stream.readLine().trimmed();
				if((lastq = line.lastIndexOf('"')) == -1)
					continue;
				
				parinfo = line.mid(lastq+2).split(' ');
				m_listParams.append( GameParam::createObject(line.mid(1, lastq-1), parinfo) );
			}
		}
	}
	
	m_state = StateRooms;
	update();
}

void MainWindow::sendLogout()
{
	PostData data;
	data.set("$FORMAT$", "ServerLogout");
	data.set("$VER$", PROTOCOL_VERSION);
	data.set("PLAYER", m_mapLogin.value("NAME"));
	data.set("ID", m_strID);
	data.set("T", QString::number(m_nPing));
	
	printStatus(QString::fromUtf8("Odhlašuji se"));
	sendPostQuery(m_url, data.str(), RequestLogout);
	
	m_state = StateExiting;
}

void MainWindow::printError(QString error)
{
	//QMessageBox::warning(this, "", error);
	textChat->insertHtml(QString("[%1] <font color=\"red\"><b>*** %2</b></font><br>").arg(QTime::currentTime().toString("hh:mm:ss")).arg(error));
	textChat->ensureCursorVisible();
}

void MainWindow::printStatus(QString msg)
{
	//QMessageBox::information(this, "", msg);
	textChat->insertHtml(QString("[%1] <i>*** %2</i><br>").arg(QTime::currentTime().toString("hh:mm:ss")).arg(msg));
	textChat->ensureCursorVisible();
}

void MainWindow::doneLogout(QBuffer& /*buffer*/, bool /*error*/)
{
	close();
}

void MainWindow::sendUserList()
{
	PostData data;
	
	data.set("$FORMAT$", "UserList");
	data.set("$VER$", PROTOCOL_VERSION);
	data.set("PLAYER", m_mapLogin.value("NAME"));
	data.set("ID", m_strID);
	data.set("T", QString::number(m_nPing));
	
	sendPostQuery(m_url, data.str(), RequestUserList);
}

void MainWindow::doneUserList(QBuffer& buffer, bool error)
{
	if(error)
		return;
	
	QTextStream stream(&buffer);
	QString line;
	QList<UserInfo> users;
	
	stream.setCodec(m_codec);
	
	while(!stream.atEnd())
	{
		UserInfo info;
		QStringList list;
		line = stream.readLine().trimmed();
		
		if(line[0] == '$')
			continue;
		
		list = line.split('\t');
		if(list.size() < 5)
			continue;
		
		info.name = list[0];
		info.ping = list[1].toLong();
		info.state = (UserInfo::State) list[2].toLong();
		info.room = list[4].toLong();
		
		users.append(info);
	}
	
	updateUserList(users);
}

void MainWindow::sendGameList()
{
	PostData data;
	
	data.set("$FORMAT$", "GameList");
	data.set("$VER$", PROTOCOL_VERSION);
	data.set("PLAYER", m_mapLogin.value("NAME"));
	data.set("ID", m_strID);
	data.set("T", QString::number(m_nPing));
	
	sendPostQuery(m_url, data.str(), RequestGameList);
}

void MainWindow::doneGameList(QBuffer& buffer, bool error)
{
	if(error)
		return;
	
	QTextStream stream(&buffer);
	QString line;
	QList<GameInfo> games;
	int empty = -1;
	
	stream.setCodec(m_codec);
	
	while(!stream.atEnd())
	{
		GameInfo info;
		QStringList list;
		line = stream.readLine().trimmed();
		
		if(line[0] == '$')
			continue;
		
		list = line.split('\t');
		if(list.size() < 7)
			continue;
		
		info.id = list[0].toLong();
		info.name = list[1];
		info.gameType = list[2];
		info.gameState = (GameInfo::State) list[3].toLong();
		info.maxPlayers = list[4].toLong();
		info.players = list[5].toLong();
		info.gameTime = list[6].toLong();
		
		if(info.gameState == GameInfo::StateEmpty)
			empty = info.id;
		
		games.append(info);
	}
	
	if(m_bWaitForRoom && empty != -1)
	{
		toggleWaitForRoom();
		sendSetGame(empty, "@@@");
	}
	
	updateGameList(games);
}

void MainWindow::updateUserList(QList<UserInfo>& users)
{
	qDebug() << "updateUserList:" << users.size() << "users";
	
	TreeItem* pWatching = m_pUsersModel->rootItem->child(0); // prihlizejici
	TreeItem* pPlaying = m_pUsersModel->rootItem->child(1); // hrajici
	TreeItem* pWaiting = m_pUsersModel->rootItem->child(2); // cekajici
	int nWatching=pWatching->childCount(), nPlaying=pPlaying->childCount(), nWaiting=pWaiting->childCount();
	
	// prihlizejici
	foreach(UserInfo user, users)
	{
		TreeItem* pItem;
		QStringList data;
		
		if(user.state != UserInfo::StateWatching)
			continue;
		
		if(nWatching <= 0)
		{
			pItem = new TreeItem(pWatching);
			pWatching->appendChild(pItem);
		}
		else
			pItem = pWatching->child( pWatching->childCount() - nWatching );
		nWatching--;
		
		data << colourNick(user.name) << QString("%1 ms").arg(user.ping)
				<< ((user.room != -1) ? QString::number(user.room) : QString("--"));
		pItem->setData(data);
	}
	while(nWatching-- > 0)
		pWatching->removeChild( pWatching->child(pWatching->childCount()-1) );
	
	// hrajici
	foreach(UserInfo user, users)
	{
		TreeItem* pItem;
		QStringList data;
		
		if(user.state != UserInfo::StatePlaying)
			continue;
		
		if(nPlaying <= 0)
		{
			pItem = new TreeItem(pPlaying);
			pPlaying->appendChild(pItem);
		}
		else
			pItem = pPlaying->child( pPlaying->childCount() - nPlaying );
		nPlaying--;
		
		data << colourNick(user.name) << QString("%1 ms").arg(user.ping)
				<< ((user.room != -1) ? QString::number(user.room) : QString("--"));
		pItem->setData(data);
	}
	while(nPlaying-- > 0)
		pPlaying->removeChild( pPlaying->child(pPlaying->childCount()-1) );
	
	// cekajici
	foreach(UserInfo user, users)
	{
		TreeItem* pItem;
		QStringList data;
		
		if(user.state != UserInfo::StateWaiting)
			continue;
		
		if(nWaiting <= 0)
		{
			pItem = new TreeItem(pWaiting);
			pWaiting->appendChild(pItem);
		}
		else
			pItem = pWaiting->child( pWaiting->childCount() - nWaiting );
		nWaiting--;
		
		data << colourNick(user.name) << QString("%1 ms").arg(user.ping)
				<< ((user.room != -1) ? QString::number(user.room) : QString("--"));
		pItem->setData(data);
	}
	while(nWaiting-- > 0)
		pWaiting->removeChild( pWaiting->child(pWaiting->childCount()-1) );
	
	m_pUsersModel->doReset();
	for(int i=0;i<3;i++)
		treeUsers->expand( m_pUsersModel->index(i, 0, QModelIndex()) );
}

void MainWindow::updateGameList(QList<GameInfo>& games)
{
	TreeItem* rootItem = m_pGamesModel->rootItem;
	int nGames = rootItem->childCount();
	bool emitReset = false;
	
	qDebug() << "Processing" << games.size() << "rooms";
	
	foreach(GameInfo game, games)
	{
		GameTreeItem* pItem;
		QStringList data;
		QIcon icon;
		bool bFull = game.players >= game.maxPlayers;
		
		if(nGames <= 0)
		{
			pItem = new GameTreeItem(rootItem);
			rootItem->appendChild(pItem);
			
			emitReset = true;
		}
		else
			pItem = (GameTreeItem*) rootItem->child( rootItem->childCount() - nGames );
		
		nGames--;
		
		pItem->m_bPassworded = false;
		
		data << QString::number(game.id) << game.name /*<< game.gameType*/;
		
		switch(game.gameState)
		{
		case GameInfo::StateEmpty:
			data << QString::fromUtf8("Hra neběží"); break;
		case GameInfo::StateWaiting:
			data << QString::fromUtf8("Hra čeká");
			icon = QIcon((bFull) ? ":/img/nopassword_full.png" : ":/img/nopassword.png");
			break;
		case GameInfo::StateWaitingPassword:
			data << QString::fromUtf8("Hra čeká s heslem");
			icon = QIcon((bFull) ? ":/img/password_full.png" : ":/img/password.png");
			pItem->m_bPassworded = true;
			break;
		case GameInfo::StatePlaying:
			data << QString::fromUtf8("Hra běží");
			icon = QIcon((bFull) ? ":/img/nopassword_full.png" : ":/img/nopassword.png");
			break;
		case GameInfo::StatePlayingPassword:
			data << QString::fromUtf8("Hra běží s heslem");
			icon = QIcon((bFull) ? ":/img/password_full.png" : ":/img/password.png");
			pItem->m_bPassworded = true;
			break;
		case GameInfo::StateTerminating:
			data << QString::fromUtf8("Server se ukončuje"); break;
		default:
			data << QString();
		}
		
		if(game.gameTime != -1)
			data << QString("%1h %2m").arg(game.gameTime/60).arg(game.gameTime%60);
		else
			data << "--";
		data << QString("%1/%2").arg(game.players).arg(game.maxPlayers);
		
		pItem->setData(data);
		pItem->setIcon(icon);
	}
	while(nGames-- > 0)
	{
		rootItem->removeChild( rootItem->child(rootItem->childCount()-1) );
		emitReset = true;
	}
	//if(emitReset)
		m_pGamesModel->doReset();
}

void MainWindow::gameDoubleClicked(const QModelIndex& game)
{
	if(m_nGame != -1)
		return;
	
	GameTreeItem* item = static_cast<GameTreeItem*>(game.internalPointer());
	int roomid = item->data(0).toString().toLong();
	QString password;
	
	qDebug() << "Switching to RoomID" << roomid;
	
	if(item->m_bPassworded)
	{
		bool ok;
		password = QInputDialog::getText(this, "Zadejte heslo",
			QString::fromUtf8("Tato místnost je chráněna heslem, prosím zadejte heslo."), QLineEdit::Password,
			QString(), &ok);
		
		if(!ok)
			return;
	}
	else
		password = "@@@";
	
	sendSetGame(roomid, password);
}

void MainWindow::sendSetGame(int roomid, QString password)
{
	PostData data;
	
	data.set("$FORMAT$", "ServerSetGame");
	data.set("$VER$", "1.02");
	data.set("PLAYER", m_mapLogin.value("NAME"));
	data.set("ID", m_strID);
	data.set("T", QString::number(m_nPing));
	data.set("GAME", QString::number(roomid));
	data.set("PASSWORD", password);
	
	m_nGame = roomid;
	
	sendPostQuery(m_url, data.str(), RequestSetGame);
}

void MainWindow::doneSetGame(QBuffer& buffer, bool error)
{
	if(error)
		return;
	
	QMap<QString,QString> info;
	
	parseKeyValue(&buffer, info);
	if(info.value("GAME") == "-1" && m_nGame != -1)
	{
		printError(QString::fromUtf8("Bylo zadáno nesprávné heslo"));
		QMessageBox::warning(this, QString::fromUtf8("Vstup do místnosti"),
				     QString::fromUtf8("Bylo zadáno nesprávné heslo"));
		m_nGame = -1;
	}
	else
	{
		printStatus(QString::fromUtf8("Byla přepnuta místnost"));
		// Přepnout zobrazení
		if(m_nGame == -1)
		{
			stackedGame->setCurrentIndex(0);
			m_state = StateRooms;
			
			m_pChat->leaveSubroom();
		}
		else
		{
			stackedGame->setCurrentIndex(1);
			m_state = StateGame;
			m_pServerModel->rootItem->removeAll();
			m_pGameServerModel->rootItem->removeAll();
			m_pServerModel->doReset();
			m_pGameServerModel->doReset();
			
			m_pChat->joinSubroom(m_nGame);
			
			m_gameInfo.bAdmin = info.value("ADMIN").toLong() != 0;
			m_gameInfo.bServerReady = true;
		}
		update();
	}
}

void MainWindow::sendGameAll(AllRequest requestType)
{
	PostData data;
	QByteArray request;
	
	data.set("$FORMAT$", "GameAll");
	data.set("$VER$", "1.02");
	data.set("PLAYER", m_mapLogin.value("NAME"));
	data.set("ID", m_strID);
	data.set("T", QString::number(m_nPing));
	data.set("GAME", QString::number(m_nGame));
	
	request = data.str();
	
	if(m_gameInfo.bAdmin)
	{
		if(requestType == AllSetParams)
		{
			request += m_codec->fromUnicode(QString("PARAMS\r\n"
					"\"%1\" \r\n"
					"\"%2\" \r\n"
					"\"%3\" \r\n"
					"\"%4\" \r\n"
					"%5 \r\n"
					"%6 \r\n"
					"%7 \r\n"
					"\"%8\" \r\n"
					"%9 \r\n")
					.arg(m_gameInfo.strName)
					.arg(m_gameInfo.strDescr) // popisek hry
					.arg(m_gameInfo.strGameMap)
					.arg(m_gameInfo.strRoomPassword)
					.arg(m_gameInfo.nMaxPlayers)
					.arg(int(m_gameInfo.bCanJoinAfterStart))
					.arg(/*info.nUnknown*/0)
					.arg(/*info.strUnknown.c_str()*/ "")
					.arg(m_gameInfo.vecParamValues.size()));
			
			foreach(QString value, m_gameInfo.vecParamValues)
			{
				request += value;
				request += "\r\n";
			}
		}
		else if(requestType == AllStart)
		{
			request += "START\r\n";
		}
	}
	
	sendPostQuery(m_url,request,RequestGameAll);
}

void MainWindow::doneGameAll(QBuffer& buffer, bool error)
{
	if(error)
		return;
	
	qDebug() << "doneGameAll()";
	
	QString line;
	QTextStream stream(&buffer);
	stream.setCodec(m_codec);
	
	for(int i=0;i<15;i++)
	{
		QString unquoted;
		line = stream.readLine().trimmed();
		
		if(line[0] == '"')
			unquoted = line.mid(1, line.size()-2);
		
		switch(i)
		{
		case 2:
			m_gameInfo.strName = unquoted; break;
		case 3:
			m_gameInfo.strDescr = unquoted; break;
		case 4:
			m_gameInfo.strIP = unquoted; break;
		case 5:
			m_gameInfo.nPort = line.toUShort(); break;
		case 6:
			m_gameInfo.strGameMap = unquoted; break;
		case 7:
			m_gameInfo.strPassword = unquoted; break;
		case 9:
			m_gameInfo.nMaxPlayers = line.toLong(); break;
		case 11:
			{
				bool bPrevState = m_gameInfo.bServerReady;
				m_gameInfo.bServerReady = line.toLong() != 0;
				
				if(m_gameInfo.bServerReady && !bPrevState)
					printStatus(QString::fromUtf8("Server byl spuštěn!"));
				break;
			}
		case 14:
			m_gameInfo.nGameTime = line.toLong(); break;
		}
	}
	
	// a jdeme na parametry hry
	long nParams, nPlayers;
	
	line = stream.readLine().trimmed();
	nParams = line.toLong();
	m_gameInfo.vecParamValues.clear();
	
	for(long i=0;i<nParams;i++)
		m_gameInfo.vecParamValues.append(stream.readLine().trimmed());
	
	line = stream.readLine().trimmed();
	nPlayers = line.toLong();
	m_gameInfo.listPlayers.clear();
	
	for(long i=0;i<nPlayers;i++)
	{
		int pos,pos2;
		PlayerInfo info;
		
		line = stream.readLine().trimmed();
		
		// parsovani uzivatelu - to krucinal nemuzou oddelovat pomoci \t i tady?!
		if(line[0] != '"')
			continue; // neco je spatne
		
		pos = line.indexOf('"', 1);
		info.strName = line.mid(1, pos-1);
		pos2 = line.indexOf(' ', pos+2);
		info.nPing = line.mid(pos+2, pos2-pos-2).toLong();
		pos = line.lastIndexOf('"', line.size()-2);
		info.bChangeRight = line.mid(pos2+1)[0] == '1';
		info.strIP = line.mid(pos+1, line.size()-pos-1);
		
		m_gameInfo.listPlayers.append(info);
	}
	
	updateGameAll();
}

void MainWindow::updateGameAll()
{
	if(m_gameInfo.nGameTime != -1)
		labelGameTime->setText(QString::fromUtf8("<b>Délka hry</b>: %1h %2m").arg(m_gameInfo.nGameTime/60).arg(m_gameInfo.nGameTime%60));
	else
		labelGameTime->setText(QString::fromUtf8("<b>Délka hry</b>: --"));
	labelGameType->setText(QString("<b>Typ hry</b>: %1").arg(m_gameInfo.strGameMap));
	
	pushParameters->setEnabled(true);
	if(m_gameInfo.bServerReady)
	{
		labelStatus->setText(QString::fromUtf8("<b><font color=\"green\">Server je spuštěn</font></b>"));
		pushStart->setText("Spustit hru!");
		pushStart->setEnabled(true);
	}
	else if(m_gameInfo.bAdmin)
	{
		labelStatus->setText(QString::fromUtf8("<b><font color=\"yellow\">Server neběží, můžete jej spustit</font></b>"));
		pushStart->setText("Spustit server");
		pushStart->setEnabled(true);
	}
	else
	{
		labelStatus->setText(QString::fromUtf8("<b><font color=\"red\">Server neběží</font></b>"));
		pushStart->setText("Spustit server");
		pushStart->setEnabled(false);
	}
	
	TreeItem* rootItem = m_pServerModel->rootItem;
	int nPlayers = rootItem->childCount();
	
	foreach(PlayerInfo player, m_gameInfo.listPlayers)
	{
		TreeItem* pItem;
		QStringList data;
		
		if(nPlayers <= 0)
		{
			pItem = new TreeItem(rootItem);
			rootItem->appendChild(pItem);
		}
		else
			pItem = rootItem->child( rootItem->childCount() - nPlayers );
		nPlayers--;
		
		data << colourNick(player.strName) << QString("%1 ms").arg(player.nPing)
				<< ((player.bChangeRight) ? "Ano" : "Ne");
		pItem->setData(data);
	}
	while(nPlayers-- > 0)
		rootItem->removeChild( rootItem->child(rootItem->childCount()-1) );
	
	m_pServerModel->doReset();
}

void MainWindow::start()
{
	if(!haveGameConfig())
	{
		printError(QString::fromUtf8("Klient nemá konfigurační data a proto nemůže pokračovat"));
	}
	if(m_gameInfo.bServerReady)
	{
		// spustit hru
		qDebug() << "PARAMS:" << m_mapConnect.value("PARAMS");
		if(m_mapConnect.value("PARAMS") == "-")
		{
			QMessageBox::information(this, "Spustit hru",
					QString::fromUtf8("Tato hra vyžaduje ruční spuštění. Zde jsou údaje:\n\n"
							"IP: %1\nPort: %2\nHeslo: %3").arg(m_gameInfo.strIP).arg(m_gameInfo.nPort)
							.arg(m_gameInfo.strPassword));
		}
		else
		{
			if(m_process.state() != QProcess::NotRunning)
			{
				QMessageBox::warning(this, "Spustit hru", QString::fromUtf8("Hra již běží!"));
			}
			else
			{
				QString command;
				command = getStartCommand(m_mapConnect.value("PARAMS"), m_mapConnect.value("TYPE"),
						m_gameInfo.strIP, QString::number(m_gameInfo.nPort), m_mapLogin.value("NAME"),
						m_gameInfo.strPassword);
				
				m_nCycle = 0;
				m_process.start(command, QIODevice::NotOpen);
			}
		}
	}
	else if(m_gameInfo.bAdmin)
	{
		printStatus(QString::fromUtf8("Odesílám příkaz pro spuštění serveru, prosím vyčkejte změny stavu"));
		sendGameAll(AllStart);
	}
}

void MainWindow::processError(QProcess::ProcessError error)
{
	qDebug() << "processError()";
	if(error == QProcess::FailedToStart)
	{
		QMessageBox::critical(this, "Spustit hru",
				      QString::fromUtf8("Nepodařilo se spustit hru, je správně nastaven příkaz pro spuštění?"));
	}
}

void MainWindow::processStateChanged(QProcess::ProcessState newState)
{
	//if(newState == QProcess::Running)
	//	printStatus(QString::fromUtf8("Proces hry byl spuštěn"));
	//else
	if(newState == QProcess::NotRunning)
		printStatus(QString::fromUtf8("Proces hry byl ukončen"));
}

bool MainWindow::terminateGame()
{
	if(m_process.state() != QProcess::NotRunning)
	{
		if(QMessageBox::warning(this, QString::fromUtf8("Opustit místnost"),
		   QString::fromUtf8("Hra stále běží, chcete hru ukončit?"), "Ano", "Ne") == 0)
		{
			m_process.terminate();
			
			if(!m_process.waitForFinished(5000))
			{
				if(QMessageBox::critical(this, QString::fromUtf8("Opustit místnost"),
				   QString::fromUtf8("Proces neodpovídá, chcete zabít proces?"), "Ano", "Ne") == 0)
				{
					m_process.kill();
				}
				else
					return false;
			}
		}
		else
			return false;
	}
	return true;
}

void MainWindow::switchBack()
{
	if(terminateGame())
		sendSetGame();
}

void MainWindow::configDialog()
{
	if(ConfigDlg(this).exec() == QDialog::Accepted)
	{
		QSettings settings;
		m_timer.start(1000 * settings.value("general/refreshInterval", 10).toInt());
	}
}

void MainWindow::userMessage(QString user, QString text)
{
	struct Smiley
	{
		const char* str[5];
		const char* path;
	};
	
	const Smiley smileys[] = {
		{ { ":-)", ":)", 0}, ":/smileys/sm_smile.png" },
		{ { ":-(", ":(", 0}, ":/smileys/sm_sad.png" },
		{ { ">:-(", ">:(", 0}, ":/smileys/sm_angry.png" },
		{ { "B-)", 0}, ":/smileys/sm_brejle.png" },
		{ { "8-)", 0}, ":/smileys/sm_crazy.png" },
		{ { ":'-(", ":'(", 0}, ":/smileys/sm_cry.png" },
		{ { ";-)", ";)", 0}, ":/smileys/sm_mrk.png" },
		{ { ":-o", ":-O", 0}, ":/smileys/sm_O.png" },
		{ { ":-|", ":|", 0}, ":/smileys/sm_serious.png" },
		{ { ":-D", ":D", 0}, ":/smileys/sm_smich.png" },
		{ { ":-p", ":-P", ":P", ":p", 0}, ":/smileys/sm_smich.png" },
	};
	
	if(m_settings.value("general/smileys", true).toBool())
	{
		for(unsigned i=0;i<sizeof(smileys)/sizeof(smileys[0]);i++)
		{
			QString replace = QString("<img src=\"%1\">").arg(smileys[i].path);
			for(int j=0;smileys[i].str[j];j++)
			{
				text.replace(smileys[i].str[j], replace);
			}
		}
	}
	
	textChat->insertHtml(QString("[%1] <b>%2</b>: %3<br>").arg(QTime::currentTime().toString("hh:mm:ss")).arg(user).arg(text));
	textChat->ensureCursorVisible();
}

void MainWindow::userJoin(QString user)
{
	if(m_settings.value("general/userInform", true).toBool())
	{
		textChat->insertHtml(QString::fromUtf8("[%1] <i>Vstoupil uživatel %2</i><br>").arg(QTime::currentTime().toString("hh:mm:ss")).arg(user));
		textChat->ensureCursorVisible();
	}
}

void MainWindow::userLeave(QString user, QString reason)
{
	if(m_settings.value("general/userInform", true).toBool())
	{
		textChat->insertHtml(QString::fromUtf8("[%1] <i>Odešel uživatel %2 (%3)</i><br>")
				.arg(QTime::currentTime().toString("hh:mm:ss")).arg(user).arg(reason));
		textChat->ensureCursorVisible();
	}
}

void MainWindow::chatSend()
{
	if(m_pChat)
	{
		m_pChat->sendMessage(lineChat->text());
		lineChat->clear();
	}
}

void MainWindow::showParameters()
{
	ParamsDlg dlg(this, m_listParams, m_gameInfo, m_listMaps,
		      QPair<int,int>(m_mapConnect.value("MINMAX").toLong(),m_mapConnect.value("MAXMAX").toLong()));
	if(dlg.exec() == QDialog::Accepted)
		sendGameAll(AllSetParams); 
}

QString MainWindow::colourNick(QString nick)
{
	/*if(m_settings.value("general/colouredNicks", true).toBool())
	{
		const char* entities[10] = {"black", "red", "green", "yellow", "blue", "cyan", "pink", "white", "black", "gray"};
		
		for(int i=0;i<10;i++)
		{
			nick.replace(QString("^%1").arg(i), QString("<font color=\"%1\">").arg(entities[i]));
		}
	}*/
	
	return nick;
}

void MainWindow::toggleWaitForRoom()
{
	m_bWaitForRoom = !m_bWaitForRoom;
	actionWaitForRoom->setChecked(m_bWaitForRoom);
}
