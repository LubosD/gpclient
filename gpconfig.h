#ifndef GPCONFIG_H
#define GPCONFIG_H
#include <QString>
#include <QStringList>
#include <QWidget>
#include <QBuffer>

struct PlayerInfo
{
	QString strName;
	int nPing;
	bool bChangeRight;
	QString strIP;
};
struct GameAllInfo
{
	QString strName, strIP, strGameMap, strPassword, strGameList, strRoomPassword, strDescr;
	unsigned short nPort;
	int nMaxPlayers, nGameTime;
	bool bCanJoinAfterStart, bServerReady;
	QList<PlayerInfo> listPlayers;
	QStringList vecParamValues;
	bool bAdmin; // ma nas user pravo na zmenu parametru mistnosti?
};

class GameParam
{
protected:
	GameParam(QString strText) : m_strText(strText) {}
	virtual ~GameParam() {}
	
	QString m_strText;
public:
	static GameParam* createObject(QString text, const QStringList& params);
	QString getText() { return m_strText; }
	virtual void createWidget(QString value, QWidget** left, QWidget** right) = 0;
	virtual QString getWidgetValue(QWidget* pWidget) = 0;
};

class GameParamInputRange : public GameParam
{
public:
	GameParamInputRange(QString text, const QStringList& params);
	virtual ~GameParamInputRange() {}
	virtual void createWidget(QString value, QWidget** left, QWidget** right);
	virtual QString getWidgetValue(QWidget* pWidget);
private:
	int m_min,m_max;
};

class GameParamBoolean : public GameParam
{
public:
	GameParamBoolean(QString text);
	virtual ~GameParamBoolean() {}
	virtual void createWidget(QString value, QWidget** left, QWidget** right);
	virtual QString getWidgetValue(QWidget* pWidget);
};

struct GameConfig
{
	QString id, command, name;
	int protoid, poffset;
};

void processGameConfig(QBuffer& buffer);
bool haveGameConfig();
bool haveGameConfig(QString id);
QList<GameConfig>& getGameConfig();
QString getStartCommand(QString format, QString id, QString ip, QString port, QString nick, QString password);

#endif
