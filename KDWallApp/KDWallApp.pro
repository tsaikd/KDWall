TEMPLATE = app
TARGET = KDWallApp

BASEDIR = $${PWD}/..
SRCDIR = $${BASEDIR}/src
INCDIR = $${BASEDIR}/include
LANGDIR = $${BASEDIR}/lang

QT *= sql

win32 {
	LIBS += User32.lib
}

TRANSLATIONS += $${LANGDIR}/KDWall_zh_TW.ts
RESOURCES += $${BASEDIR}/KDWall.qrc
RC_FILE = KDWallApp.rc

INCLUDEPATH += $${SRCDIR}
PRECOMPILED_HEADER += $${SRCDIR}/stable.h

SOURCES += main.cpp
SOURCES += $${SRCDIR}/Debug.cpp
HEADERS += $${SRCDIR}/WinMainApp.h
SOURCES += $${SRCDIR}/WinMainApp.cpp
SOURCES += $${SRCDIR}/ConfMainApp.cpp
HEADERS += $${SRCDIR}/WinConfMainApp.h
SOURCES += $${SRCDIR}/WinConfMainApp.cpp
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
