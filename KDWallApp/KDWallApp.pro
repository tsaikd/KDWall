TEMPLATE = app
TARGET = KDWall

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

# CppExtLib
INCLUDEPATH *= ../exLib/CppExtLib/src
SOURCES += ../exLib/CppExtLib/src/ExtDebug.cpp

# QtExtLib
include(../exLib/QtExtLib/gui.pri)

SOURCES += main.cpp
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
