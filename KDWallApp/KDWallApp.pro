TEMPLATE = app
TARGET = KDWallApp

BASEDIR = $${PWD}/..
SRCDIR = $${BASEDIR}/src
INCDIR = $${BASEDIR}/include

QT += sql

include($${INCDIR}/qtsingleapplication/qtsingleapplication.pri)

win32 {
	LIBS += User32.lib
}

CONFIG(debug, debug|release) {
	TARGET = ../$$join(TARGET,,,d)
	CONFIG -= release
	CONFIG += debug
	DEFINES += DEBUG
	DEFINES -= NDEBUG
} else {
	TARGET = ../$${TARGET}
	CONFIG -= debug
	CONFIG += release
	DEFINES -= DEBUG
	DEFINES += NDEBUG
}

#TRANSLATIONS += $${BASEDIR}/lang/MotionManager_zh_TW.ts
#RESOURCES += $${BASEDIR}/MotionManager.qrc

PRECOMPILED_HEADER += $${BASEDIR}/src/stable.h

SOURCES += $${SRCDIR}/Debug.cpp
SOURCES += $${SRCDIR}/mainApp.cpp
HEADERS += $${SRCDIR}/WinMainApp.h
SOURCES += $${SRCDIR}/WinMainApp.cpp
SOURCES += $${SRCDIR}/ConfMainApp.cpp
SOURCES += $${SRCDIR}/Util.cpp
SOURCES += $${SRCDIR}/DBMgr.cpp
HEADERS += $${SRCDIR}/WallMgr.h
SOURCES += $${SRCDIR}/WallMgr.cpp
HEADERS += $${SRCDIR}/PicFinder.h
SOURCES += $${SRCDIR}/PicFinder.cpp
HEADERS += $${SRCDIR}/TreePicDir.h
SOURCES += $${SRCDIR}/TreePicDir.cpp
