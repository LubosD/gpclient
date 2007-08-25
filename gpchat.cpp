#include "gpchat.h"
#include <QtDebug>

GPChat::GPChat(QString host, unsigned short port, QString user, int room, QString password)
	: m_nLastRoom(-1), m_nLastSubroom(-1), m_nFirstRoom(room), m_status(StatusNone)
{
	connect(&m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
	connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
	connect(&m_socket, SIGNAL(connected()), this, SLOT(connected()));
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(ping()));
	
	m_socket.connectToHost(host, port);
	m_codec = QTextCodec::codecForName("Windows-1250");
	m_strUser = m_codec->fromUnicode(user);
	m_strPassword = m_codec->fromUnicode(password);
	m_timer.start(20000);
}

void GPChat::error(QAbstractSocket::SocketError /*socketError*/)
{
	emit errorMessage(QString("Chat: chyba socketu: ") + m_socket.errorString());
}

void GPChat::connected()
{
	m_status = StatusHello;
}

void GPChat::disconnected()
{
	emit errorMessage(QString::fromUtf8("Chat: připojení k serveru ztraceno"));
	m_status = StatusNone;
}

void GPChat::ping()
{
	if(m_status == StatusConnected)
	{
		ChatCmd cmd;
		
		cmd.name = "P";
		cmd.value = "PING";
		
		m_socket.write(cmd2data(cmd));
	}
}

void GPChat::sendMessage(QString message)
{
	if(m_status != StatusConnected)
		emit errorMessage(QString::fromUtf8("Zprávu nelze nyní odeslat, zkuste to později"));
	else
	{
		ChatCmd cmd;
		
		cmd.name = "RUSER_MESSAGE";
		cmd.value = "Name";
		
		cmd.listParams << m_strRoom << "" << m_codec->fromUnicode(message) << "";
		m_socket.write(cmd2data(cmd));
	}
}

void GPChat::joinRoom(int room, int subroom)
{
	ChatCmd cmd;
	qDebug() << "joinRoom" << room << subroom;
	
	if(m_nLastRoom != -1 && m_nLastSubroom != -1 && (room != m_nLastRoom || subroom != m_nLastSubroom))
	{
		// leave
		m_strRoom = QByteArray().append(QString("GP_%1_%2").arg(m_nLastRoom).arg(m_nLastSubroom));
		
		cmd.name = "RLEAVE_ROOM";
		cmd.value = "Name";
		cmd.listParams << m_strRoom << "";
		
		m_status = StatusLeave;
	}
	else
	{
		QString strRoom;
		
		cmd.name = "RJOIN_ROOM";
		cmd.value = "Name";
		
		if(subroom != -1)
			m_strRoom = QByteArray().append(QString("GP_%1_%2").arg(room).arg(subroom));
		else
			m_strRoom = QByteArray().append(QString("GP_%1").arg(room));
		
		cmd.listParams << m_strRoom << "1" << "1" << "0" << "1" << "1" << "";
		
		m_status = StatusJoin;
	}
	
	m_nLastRoom = room;
	m_nLastSubroom = subroom;
	m_socket.write(cmd2data(cmd));
}

void GPChat::readyRead()
{
	QByteArray array;
	ChatCmd cmd;
	
	while(true)
	{
		array += m_socket.readAll();
		qDebug() << "Chat: Received" << array.size() << "bytes";
		
		if(array.size() && array[array.size()-1] == '\xa6')
		{
			qDebug() << "Chat: done receiving";
			break;
 		}
		m_socket.waitForReadyRead(100);
	}
	
	cmd = data2cmd(array);
	
	qDebug() << "***Chat command:" << cmd.name;
	
	if(m_status == StatusHello)
	{
		ChatCmd lcmd;
		lcmd.name = "RUSER_LOGIN";
		lcmd.value = m_codec->fromUnicode(m_strUser);
		lcmd.listParams << QByteArray().append(QString::number(rand())) << "0" << cmd.value << "0" << "1" << "1" << m_strPassword << "";
				
		qDebug() << "Logging in, ID" << cmd.value;
				
		m_status = StatusLogin;
		m_socket.write(cmd2data(lcmd));
	}
	else if(cmd.name == "S0")
	{
		switch(m_status)
		{
		case StatusLogin:
			if(cmd.value.toLong())
			{
				//emit statusMessage(QString::fromUtf8("Přihlášen do chatu..."));
				joinRoom(m_nFirstRoom);
			}
			else
				emit errorMessage(QString::fromUtf8("Nepodařilo se přihlásit k chatu"));
			break;
		case StatusJoin:
			qDebug() << "Join room result:" << cmd.value;
			if(m_nLastSubroom == -1)
				emit statusMessage(QString::fromUtf8("Vstoupil jste do místnosti chatu"));
			else
				emit statusMessage(QString::fromUtf8("Vstoupil jste do místnosti chatu (%1)").arg(m_nLastSubroom));
			m_status = StatusConnected;
			break;
		case StatusLeave:
			joinRoom(m_nLastRoom, m_nLastSubroom);
			break;
		default:
			;
		}
	}
	else if(cmd.name == "P")
	{
		// ping
	}
	else if(cmd.name == "MMESSAGE")
	{
		QString strUser = m_codec->toUnicode(cmd.listParams[1]);
		QString strMessage = m_codec->toUnicode(cmd.listParams[3]);
		
		if(cmd.listParams[0] == "UA") // login
			emit userJoin(strUser);
		else if(cmd.listParams[0] == "UD") // logout
			emit userLeave(strUser, strMessage);
		else if(cmd.listParams[0] == "UM") // message
			emit userMessage(strUser, strMessage);
		else
			qDebug() << "Chat: Unknown message type" << cmd.listParams[0];
	}
	else
		qDebug() << "Chat: Unknown data type" << cmd.name;
}

ChatCmd GPChat::data2cmd(QByteArray array)
{
	int i = 0;
	ChatCmd cmd;
	
	while(array[i] != ':')
		cmd.name += array[i++];
	i++;
	
	while(array[i] != '\xA6' && array[i] != '\xAB')
		cmd.value += array[i++];
	
	while(array[i++] != '\xA6')
	{
		QByteArray param;
		
		while(array[i] != '\xA6' && array[i] != '\xAB')
			param += array[i++];
		
		cmd.listParams << param;
	}
	
	return cmd;
}

QByteArray GPChat::cmd2data(ChatCmd& cmd)
{
	QByteArray output;
	output += cmd.name;
	output += ':';
	output += cmd.value;
	
	if(!cmd.listParams.empty())
	{
		foreach(QString param, cmd.listParams)
		{
			output += '\xab';
			output += param;
		}
		output += '\xbb';
	}
	
	output += '\xa6';
	return output;
}

