#include <QApplication>
#include <QMessageBox>
#include "MainWindow.h"
#include "palette.h"
#include <QSettings>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	MainWindow* wnd;
	
	QCoreApplication::setOrganizationName("LubosD");
	QCoreApplication::setOrganizationDomain("dolezel.info");
	QCoreApplication::setApplicationName("gpclient");
	
	initPalettes();
	atexit(freePalettes);
	
	try
	{
		QSettings settings;
		
		if(argc != 2)
		{
			throw QString::fromUtf8("GamePark klient musí být spuštěn s GP1 souborem jako parametr (stejně jako oficiální klient).\n"
					"Zkuste asociovat GP1 soubory k tomuto programu ve svém prohlížeči.");
		}
		
		setApplyPalette(settings.value("general/gpColors", true).toBool());
		wnd = new MainWindow(argv[1]);
	}
	catch(const QString& error)
	{
		QMessageBox::critical(NULL, "GamePark klient", error);
		return 1;
	}
	
	wnd->showMaximized();
	return app.exec();
}

