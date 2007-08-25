#include "gpconfig.h"
#include <QtDebug>
#include <QSpinBox>
#include <QCheckBox>
#include <QTextStream>
#include <QUrl>
#include <QList>
#include <QSettings>
#include <QLabel>
#include "palette.h"

static QList<GameConfig> m_config;

GameParam* GameParam::createObject(QString text, const QStringList& params)
{
	if(params.empty())
		return 0;
	
	switch(params[0].toLong())
	{
	case 2: // input range
		if(params.size() >= 3)
			return new GameParamInputRange(text, params);
		else
		{
			qCritical("Invalid param count (GameParamInputRange), got only %d\n", params.size());
			return 0;
		}
	case 3: // boolean
		return new GameParamBoolean(text);
	default:
		return 0;
	}
}

GameParamInputRange::GameParamInputRange(QString text, const QStringList& params)
	: GameParam(text)
{
	m_min = params[1].toLong();
	m_max = params[2].toLong();
}

void GameParamInputRange::createWidget(QString value, QWidget** left, QWidget** right)
{
	QSpinBox* pBox = new QSpinBox;
	
	*left = new QLabel(m_strText);
	*right = pBox;
	
	applyPalette(*left, PaletteMain);
	applyPalette(*right, PaletteEdit);
	
	pBox->setRange(m_min,m_max);
	pBox->setValue(value.toLong());
}

QString GameParamInputRange::getWidgetValue(QWidget* pWidget)
{
	QSpinBox* pBox = (QSpinBox*) pWidget;
	return QString::number(pBox->value());
}

GameParamBoolean::GameParamBoolean(QString text)
	: GameParam(text)
{
}

void GameParamBoolean::createWidget(QString value, QWidget** left, QWidget** right)
{
	QCheckBox* box = new QCheckBox(m_strText);
	if(value.startsWith("true", Qt::CaseInsensitive))
		box->setCheckState(Qt::Checked);
	
	applyPalette(box, PaletteCheck);
	
	*right = box;
	*left = 0;
}

QString GameParamBoolean::getWidgetValue(QWidget* pWidget)
{
	QCheckBox* box = (QCheckBox*) pWidget;
	return (box->checkState() == Qt::Checked) ? "True" : "False";
}

bool haveGameConfig()
{
	return !m_config.empty();
}

void processGameConfig(QBuffer& buffer)
{
	QTextStream stream(&buffer);
	
	while(!stream.atEnd())
	{
		GameConfig conf;
		QStringList params = stream.readLine().trimmed().split('\t');
		
		if(params.size() < 5)
			continue;
		
		conf.id = params[0];
		conf.command = params[1];
		conf.name = params[2];
		conf.protoid = params[3].toLong();
		conf.poffset = params[4].toLong();
		
		m_config.append(conf);
	}
}

QList<GameConfig>& getGameConfig()
{
	return m_config;
}

QString getStartCommand(QString format, QString id, QString ip, QString port, QString nick, QString password)
{
	QString cmd = "\"", def;
	QSettings settings;
	int limpos;
	
	qDebug() << "Format string:" << format;
	format.replace(":s", "");
	format.replace("%5", "%2");
	format.replace("%6", "%3");
	
	foreach(GameConfig cfg, m_config)
	{
		if(cfg.id == id)
		{
			def = cfg.command;
			break;
		}
	}
	
	qDebug() << "Format string:" << format;
	qDebug() << "Game command:" << settings.value(QString("games/")+id, cmd).toString();
	
	cmd += settings.value(QString("games/")+id, def).toString();
	cmd += "\" ";
	format = format.arg(ip).arg(port);
	if(format.contains("%2"))
		format = format.arg(nick);
	limpos = format.indexOf("%3:.");
	if(limpos != -1)
	{
		QByteArray ascii = format.toAscii();
		int num = atoi(ascii.constData()+limpos+4);
		password.truncate( num );
		format.replace(QString("%3:.%1s").arg(num), "%3");
	}
	cmd += format.arg(password);
	
	qDebug() << "Command to execute:" << cmd;
	
	return cmd;
}

bool haveGameConfig(QString id)
{
	foreach(GameConfig cfg, m_config)
	{
		if(cfg.id == id)
			return true;
	}
	return false;
}
