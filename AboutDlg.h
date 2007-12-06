#ifndef ABOUTDLG_H
#define ABOUTDLG_H
#include "ui_AboutDlg.h"
#include <QMessageBox>
#include <QFile>
#include "palette.h"

class AboutDlg : public QDialog, public Ui_AboutDlg
{
Q_OBJECT
public:
	AboutDlg(QWidget* parent)
	: QDialog(parent)
	{
		setupUi(this);
		applyPalette(this, PaletteMain);
		applyPalette(textLicense, PaletteEdit);
		connect(pushAboutQt, SIGNAL(clicked()), this, SLOT(aboutQt()));
		
		QFile file("/usr/share/gpclient/LICENSE");
		if(!file.open(QIODevice::ReadOnly))
			file.setFileName("./LICENSE");
		if(!file.open(QIODevice::ReadOnly))
			return;
		textLicense->setPlainText(QString(file.readAll().constData()));
	}
public slots:
	void aboutQt()
	{
		QMessageBox::aboutQt(this);
	}
};

#endif
