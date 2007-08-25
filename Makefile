#############################################################################
# Makefile for building: gpclient
# Generated by qmake (2.01a) (Qt 4.3.1) on: ne srp 19 01:13:31 2007
# Project:  gpclient.pro
# Template: app
# Command: /usr/bin/qmake -unix -o Makefile gpclient.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtNetwork -I/usr/include/qt4/QtNetwork -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -I.
LINK          = g++
LFLAGS        = 
LIBS          = $(SUBLIBS)  -L/usr/lib64/qt4 -lssl -lQtGui -L/usr/lib64 -L/usr/lib64/mysql -L/usr/lib64/qt4 -L/usr/lib -lpng -lSM -lICE -lXrender -lXrandr -lXfixes -lXcursor -lXinerama -lfreetype -lfontconfig -lXext -lX11 -lQtNetwork -lQtCore -lz -lm -lrt -ldl -lpthread
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -sf
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = MainWindow.cpp \
		gpclient.cpp \
		gpconfig.cpp \
		TreeModel.cpp \
		ConfigDlg.cpp \
		palette.cpp \
		gpchat.cpp \
		ParamsDlg.cpp moc_MainWindow.cpp \
		moc_AboutDlg.cpp \
		moc_ConfigDlg.cpp \
		moc_gpchat.cpp \
		moc_ParamsDlg.cpp \
		qrc_resources.cpp
OBJECTS       = MainWindow.o \
		gpclient.o \
		gpconfig.o \
		TreeModel.o \
		ConfigDlg.o \
		palette.o \
		gpchat.o \
		ParamsDlg.o \
		moc_MainWindow.o \
		moc_AboutDlg.o \
		moc_ConfigDlg.o \
		moc_gpchat.o \
		moc_ParamsDlg.o \
		qrc_resources.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		gpclient.pro
QMAKE_TARGET  = gpclient
DESTDIR       = 
TARGET        = gpclient

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): ui_MainWindow.h ui_AboutDlg.h ui_ConfigDlg.h ui_ParamsDlg.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: gpclient.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/lib64/qt4/libQtGui.prl \
		/usr/lib64/qt4/libQtCore.prl \
		/usr/lib64/qt4/libQtNetwork.prl
	$(QMAKE) -unix -o Makefile gpclient.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/lib64/qt4/libQtGui.prl:
/usr/lib64/qt4/libQtCore.prl:
/usr/lib64/qt4/libQtNetwork.prl:
qmake:  FORCE
	@$(QMAKE) -unix -o Makefile gpclient.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/gpclient1.0.0 || $(MKDIR) .tmp/gpclient1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/gpclient1.0.0/ && $(COPY_FILE) --parents MainWindow.h gpconfig.h PostData.h AboutDlg.h TreeModel.h ConfigDlg.h palette.h gpchat.h ParamsDlg.h .tmp/gpclient1.0.0/ && $(COPY_FILE) --parents img/resources.qrc .tmp/gpclient1.0.0/ && $(COPY_FILE) --parents MainWindow.cpp gpclient.cpp gpconfig.cpp TreeModel.cpp ConfigDlg.cpp palette.cpp gpchat.cpp ParamsDlg.cpp .tmp/gpclient1.0.0/ && $(COPY_FILE) --parents MainWindow.ui AboutDlg.ui ConfigDlg.ui ParamsDlg.ui .tmp/gpclient1.0.0/ && (cd `dirname .tmp/gpclient1.0.0` && $(TAR) gpclient1.0.0.tar gpclient1.0.0 && $(COMPRESS) gpclient1.0.0.tar) && $(MOVE) `dirname .tmp/gpclient1.0.0`/gpclient1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/gpclient1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_MainWindow.cpp moc_AboutDlg.cpp moc_ConfigDlg.cpp moc_gpchat.cpp moc_ParamsDlg.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_MainWindow.cpp moc_AboutDlg.cpp moc_ConfigDlg.cpp moc_gpchat.cpp moc_ParamsDlg.cpp
moc_MainWindow.cpp: ui_MainWindow.h \
		gpconfig.h \
		gpchat.h \
		TreeModel.h \
		MainWindow.h
	/usr/bin/moc $(DEFINES) $(INCPATH) MainWindow.h -o moc_MainWindow.cpp

moc_AboutDlg.cpp: ui_AboutDlg.h \
		palette.h \
		AboutDlg.h
	/usr/bin/moc $(DEFINES) $(INCPATH) AboutDlg.h -o moc_AboutDlg.cpp

moc_ConfigDlg.cpp: ui_ConfigDlg.h \
		ConfigDlg.h
	/usr/bin/moc $(DEFINES) $(INCPATH) ConfigDlg.h -o moc_ConfigDlg.cpp

moc_gpchat.cpp: gpchat.h
	/usr/bin/moc $(DEFINES) $(INCPATH) gpchat.h -o moc_gpchat.cpp

moc_ParamsDlg.cpp: ui_ParamsDlg.h \
		gpconfig.h \
		ParamsDlg.h
	/usr/bin/moc $(DEFINES) $(INCPATH) ParamsDlg.h -o moc_ParamsDlg.cpp

compiler_rcc_make_all: qrc_resources.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_resources.cpp
qrc_resources.cpp: img/resources.qrc \
		img/password_full.png \
		img/waiting.png \
		img/gpclient_48.png \
		img/nopassword_full.png \
		img/autowait.png \
		img/config.png \
		img/nopassword.png \
		img/password.png \
		img/logout.png \
		img/gpclient_big.png \
		img/sm_mrk.png \
		img/sm_smich.png \
		img/sm_cry.png \
		img/sm_tongue.png \
		img/sm_crazy.png \
		img/sm_angry.png \
		img/sm_sad.png \
		img/sm_O.png \
		img/sm_brejle.png \
		img/sm_serious.png \
		img/sm_smile.png
	/usr/bin/rcc -name resources img/resources.qrc -o qrc_resources.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_MainWindow.h ui_AboutDlg.h ui_ConfigDlg.h ui_ParamsDlg.h
compiler_uic_clean:
	-$(DEL_FILE) ui_MainWindow.h ui_AboutDlg.h ui_ConfigDlg.h ui_ParamsDlg.h
ui_MainWindow.h: MainWindow.ui
	/usr/bin/uic MainWindow.ui -o ui_MainWindow.h

ui_AboutDlg.h: AboutDlg.ui
	/usr/bin/uic AboutDlg.ui -o ui_AboutDlg.h

ui_ConfigDlg.h: ConfigDlg.ui
	/usr/bin/uic ConfigDlg.ui -o ui_ConfigDlg.h

ui_ParamsDlg.h: ParamsDlg.ui
	/usr/bin/uic ParamsDlg.ui -o ui_ParamsDlg.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 

####### Compile

MainWindow.o: MainWindow.cpp MainWindow.h \
		ui_MainWindow.h \
		gpconfig.h \
		gpchat.h \
		TreeModel.h \
		PostData.h \
		AboutDlg.h \
		ui_AboutDlg.h \
		palette.h \
		ConfigDlg.h \
		ui_ConfigDlg.h \
		ParamsDlg.h \
		ui_ParamsDlg.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MainWindow.o MainWindow.cpp

gpclient.o: gpclient.cpp MainWindow.h \
		ui_MainWindow.h \
		gpconfig.h \
		gpchat.h \
		TreeModel.h \
		palette.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o gpclient.o gpclient.cpp

gpconfig.o: gpconfig.cpp gpconfig.h \
		palette.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o gpconfig.o gpconfig.cpp

TreeModel.o: TreeModel.cpp TreeModel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o TreeModel.o TreeModel.cpp

ConfigDlg.o: ConfigDlg.cpp ConfigDlg.h \
		ui_ConfigDlg.h \
		gpconfig.h \
		palette.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ConfigDlg.o ConfigDlg.cpp

palette.o: palette.cpp palette.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o palette.o palette.cpp

gpchat.o: gpchat.cpp gpchat.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o gpchat.o gpchat.cpp

ParamsDlg.o: ParamsDlg.cpp ParamsDlg.h \
		ui_ParamsDlg.h \
		gpconfig.h \
		palette.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ParamsDlg.o ParamsDlg.cpp

moc_MainWindow.o: moc_MainWindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_MainWindow.o moc_MainWindow.cpp

moc_AboutDlg.o: moc_AboutDlg.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_AboutDlg.o moc_AboutDlg.cpp

moc_ConfigDlg.o: moc_ConfigDlg.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ConfigDlg.o moc_ConfigDlg.cpp

moc_gpchat.o: moc_gpchat.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_gpchat.o moc_gpchat.cpp

moc_ParamsDlg.o: moc_ParamsDlg.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ParamsDlg.o moc_ParamsDlg.cpp

qrc_resources.o: qrc_resources.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qrc_resources.o qrc_resources.cpp

####### Install

install_target: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/local/bin/ || $(MKDIR) $(INSTALL_ROOT)/usr/local/bin/ 
	-$(INSTALL_PROGRAM) "$(QMAKE_TARGET)" "$(INSTALL_ROOT)/usr/local/bin/$(QMAKE_TARGET)"

uninstall_target:  FORCE
	-$(DEL_FILE) "$(INSTALL_ROOT)/usr/local/bin/$(QMAKE_TARGET)"
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/local/bin/ 


install:  install_target  FORCE

uninstall: uninstall_target   FORCE

FORCE:

