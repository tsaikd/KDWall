TEMPLATE = app
TARGET = KDWallApp

BASEDIR = $${PWD}/..
SRCDIR = $${BASEDIR}/src
INCDIR = $${BASEDIR}/include

QT += sql

win32 {
	LIBS += User32.lib
}

#TRANSLATIONS += $${BASEDIR}/lang/KDWall.ts
RESOURCES += $${BASEDIR}/KDWall.qrc
RC_FILE = $${BASEDIR}/KDWall.rc

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

include($${INCDIR}/qtsingleapplication/qtsingleapplication.pri)
include($${BASEDIR}/general.pri)
