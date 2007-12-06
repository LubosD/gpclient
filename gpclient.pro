TEMPLATE = app
TARGET = gpclient
LIBS += -lssl
QT += network

INSTALL_ROOT = /usr/bin # Kam nainstalovat

#extradata.files = gpclient
#extradata.path = $$INSTALL_ROOT
target.path = $$INSTALL_ROOT
INSTALLS += target

other_data.files = LICENSE
other_data.path = /usr/share/gpclient
INSTALLS += other_data

#CONFIG += debug
#DEPENDPATH += .
#INCLUDEPATH += .

# Input
HEADERS += MainWindow.h gpconfig.h PostData.h AboutDlg.h TreeModel.h ConfigDlg.h palette.h gpchat.h ParamsDlg.h
FORMS += MainWindow.ui AboutDlg.ui ConfigDlg.ui ParamsDlg.ui
SOURCES += MainWindow.cpp gpclient.cpp gpconfig.cpp TreeModel.cpp ConfigDlg.cpp palette.cpp gpchat.cpp ParamsDlg.cpp
RESOURCES += img/resources.qrc

