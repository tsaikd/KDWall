TEMPLATE = app
TARGET = ../KDWallApp

BASEDIR = ..
SRCDIR = $${BASEDIR}/src
INCDIR = $${BASEDIR}/include

QT += sql

include($${INCDIR}/qtsingleapplication/qtsingleapplication.pri)

win32 {
	LIBS += User32.lib
}

PRECOMPILED_HEADER += $${BASEDIR}/src/stable.h

SOURCES += $${SRCDIR}/mainApp.cpp
HEADERS += $${SRCDIR}/WinMainApp.h
SOURCES += $${SRCDIR}/WinMainApp.cpp
SOURCES += $${SRCDIR}/Util.cpp
SOURCES += $${SRCDIR}/WallMgr.cpp
HEADERS += $${SRCDIR}/PicFinder.h
SOURCES += $${SRCDIR}/PicFinder.cpp
