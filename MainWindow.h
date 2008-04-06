#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QTimer>
#include <QHttp>
#include <QBuffer>
#include <QUdpSocket>
#include <QStringList>
#include <QUrl>
#include <QTime>
#include <QVarLengthArray>
#include <QTextCodec>
#include <QSettings>
#include <QProcess>

#include "ui_MainWindow.h"
#include "gpconfig.h"
#include "gpchat.h"
#include "TreeModel.h"

class MainWindow : public QMainWindow , public Ui_MainWindow
{
Q_OBJECT
public:
	MainWindow(QString infile);
	~MainWindow();
	
	enum RequestType { RequestGameConfig, RequestGetVersion, RequestAuth, RequestConnect, RequestLogin, RequestParamsMaps, RequestUserList, RequestGameList, RequestSetGame, RequestGameAll, RequestLogout };
	
	void prepareGUI();
	void sendPostQuery(QUrl url, QByteArray postdata, RequestType type);
	void sendGetQuery(QUrl, RequestType type);
	
	static void parseKeyValue(QIODevice* pSrc, QMap<QString,QString>& out);
	static void dumpKeyValue(const QMap<QString,QString>& map);
	static QByteArray decrypt(const QByteArray& data);
	static QByteArray encrypt(const QByteArray& data);
	static QByteArray decryptString(const QString& data);
	static QString encryptString(const QByteArray& data);
	static QString generateLocalIP();
	static bool getIPMac(QString iface, QString& ip, QString& mac);
protected:
	virtual void closeEvent(QCloseEvent* event);
public slots:
	void aboutDialog();
	void configDialog();
	void toggleWaitForRoom();
	void requestFinished(int id, bool error);
	void update();
	void readPing();
	void start();
	void gameDoubleClicked(const QModelIndex& game);
	void switchBack();
	void showParameters();
	
	void printError(QString error);
	void printStatus(QString msg);
	
	void showError(QString error);
	
	void userMessage(QString user, QString text);
	void userJoin(QString user);
	void userLeave(QString user, QString reason);
	
	void chatSend();
	void processError(QProcess::ProcessError error);
	void processStateChanged(QProcess::ProcessState newState);
private:
	struct UserInfo
	{
		QString name;
		long ping, room;
		enum State { StateWatching, StateWaiting, StatePlaying } state;
	};
	struct GameInfo
	{
		long id;
		QString name, gameType;
		enum State { StateEmpty=0, StateWaiting=2, StateWaitingPassword, StatePlaying=5, StatePlayingPassword, StateTerminating } gameState;
		long gameTime; // -1 pokud se nehraje (nGameState == 0)
		long players, maxPlayers;
	};
	
	void doneGameConfig(QBuffer& buffer, bool error);
	void doneGetVersion(QBuffer& buffer, bool error);
	
	void sendAuth(QByteArray encdata);
	void doneAuth(QBuffer& buffer, bool error);
	
	void sendConnect();
	void doneConnect(QBuffer& buffer, bool error);
	
	void sendLogin();
	void doneLogin(QBuffer& buffer, bool error);
	
	void sendParamsMaps();
	void doneParamsMaps(QBuffer& buffer, bool error);
	
	void sendLogout();
	void doneLogout(QBuffer& buffer, bool error);
	
	void sendUserList();
	void doneUserList(QBuffer& buffer, bool error);
	void updateUserList(QList<UserInfo>& users);
	
	void sendGameList();
	void doneGameList(QBuffer& buffer, bool error);
	void updateGameList(QList<GameInfo>& games);
	
	void sendSetGame(int roomid = -1, QString password = "@@@");
	void doneSetGame(QBuffer& buffer, bool error);
	
	enum AllRequest { AllNone /*chci jen info*/, AllSetParams /*chci nastavit parametry*/, AllStart /*chci spustit server*/ };
	void sendGameAll(AllRequest request = AllNone);
	void doneGameAll(QBuffer& buffer, bool error);
	void updateGameAll();
	
	void updateGeneralInfo();
	bool terminateGame();
	QString colourNick(QString nick);
private:
	struct Request
	{
		RequestType type;
		QBuffer* buffer;
	};
	
	QMap<int,Request> m_mapRequests;
	QMap<QString,QString> m_mapInput, m_mapLogin, m_mapConnect;
	GameAllInfo m_gameInfo;
	QStringList m_listMaps;
	QList<GameParam*> m_listParams;
	
	// Network
	QHttp m_http;
	QUdpSocket* m_ping;
	QTime m_pingTime;
	int m_nPing, m_nCycle;
	GPChat* m_pChat;
	
	// GUI
	QLabel* m_statusLabel;
	UserTreeModel* m_pUsersModel;
	GameTreeModel* m_pGamesModel;
	TreeModel* m_pServerModel;
	TreeModel* m_pGameServerModel;
	
	QString m_strID, m_strAuth, m_strSkin;
	QByteArray m_array;
	QUrl m_url;
	int m_nGame;
	QSettings m_settings;
	QProcess m_process;
	
	enum State { StateConnecting, StateConnectingWait, StateRooms, StateGame, StateExiting } m_state;
	QTimer m_timer;
	QTextCodec* m_codec;
	
	bool m_bWaitForRoom;
};

#endif

