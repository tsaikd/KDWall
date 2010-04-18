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
RC_FILE = $${PWD}/KDWallApp.rc

INCLUDEPATH += $${SRCDIR}
PRECOMPILED_HEADER += $${SRCDIR}/stable.h

# QtExtLib
include($${BASEDIR}/exLib/QtExtLib/gui.pri)

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

DEFINES *= CPPEXTLIB_NOLINKLIB
include($${BASEDIR}/exLib/CppExtLib/CppExtLib.pri)
include($${INCDIR}/qtsingleapplication-2.6_1-opensource/src/qtsingleapplication.pri)
include($${BASEDIR}/exLib/QtExtLib/general.pri)
