#ifndef GPCHAT_H
#define GPCHAT_H
#include <QTcpSocket>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QTimer>

struct ChatCmd;
class GPChat : public QObject
{
Q_OBJECT
public:
	GPChat(QString host, unsigned short port, QString user, int room, QString password);
	void joinRoom(int room, int subroom = -1);
	void leaveSubroom() { joinRoom(m_nLastRoom); }
	void joinSubroom(int subroom) { joinRoom(m_nLastRoom,subroom); }
	void sendMessage(QString message);
public slots:
	void readyRead();
	void error(QAbstractSocket::SocketError socketError);
	void connected();
	void disconnected();
	void ping();
signals:
	void errorMessage(QString error);
	void statusMessage(QString msg);
	
	void userMessage(QString user, QString text);
	void userJoin(QString user);
	void userLeave(QString user, QString reason);
private:
	static ChatCmd data2cmd(QByteArray array);
	static QByteArray cmd2data(ChatCmd& cmd);
	
	QTcpSocket m_socket;
	QByteArray m_strRoom,m_strUser,m_strPassword;
	QTimer m_timer;
	QTextCodec* m_codec;
	int m_nLastRoom, m_nLastSubroom, m_nFirstRoom;
	int m_nID;
	enum Status { StatusNone, StatusHello, StatusLogin, StatusJoin, StatusLeave, StatusConnected } m_status;
};

struct ChatCmd
{
	QByteArray name, value;
	QList<QByteArray> listParams;
};

#endif
