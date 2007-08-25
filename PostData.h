#ifndef POSTDATA_H
#define POSTDATA_H
#include <QString>
#include <QUrl>
#include <QTextCodec>

class PostData
{
public:
	PostData()
	{
		m_codec = QTextCodec::codecForName("Windows-1250");
	}
	void set(QString left, QString right)
	{
		m_string += left;
		m_string += '=';
		m_string += right;
		m_string += "\r\n";
	}
	QByteArray str() { return m_codec->fromUnicode(m_string); }
private:
	QString m_string;
	QTextCodec* m_codec;
};

#endif
