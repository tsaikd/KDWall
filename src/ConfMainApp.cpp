#include "ConfMainApp.h"

#include "DBMgr.h"
#include "WallMgr.h"
#include "PicFinder.h"

#define LOAD_CONFIG(var, defvalue) m_##var << conf.value(#var, defvalue); m_mapConfDefValue.insert(#var, defvalue);
#define SAVE_CONFIG(var) if (m_##var == m_mapConfDefValue.value(#var)) { conf.remove(#var); } else { conf.setValue(#var, m_##var); }

static const QVariant& operator << (bool& val, const QVariant& var) {
	val = var.toBool();
	return var;
}

static const QVariant& operator << (int& val, const QVariant& var) {
	val = var.toInt();
	return var;
}

#ifdef Q_WS_X11
static const QVariant& operator << (WM& val, const QVariant& var) {
	val = (WM)var.toInt();
	return var;
}
#endif//Q_WS_X11

void QConfMainApp::_init()
{
	m_conf = new QSettings(PROJNAME".ini", QSettings::IniFormat, this);
	DECCP(QSettings, conf);
	m_mainWidget = NULL;
	m_closing = false;
	LOAD_CONFIG(initWithoutWindow, "1");

	// QDBMgr
	LOAD_CONFIG(pichist_max_num, "100");

	// QWallMgr
	LOAD_CONFIG(wall_timer_sec, "5");
	LOAD_CONFIG(max_cache_image, "5");
	LOAD_CONFIG(disable_cache_warning, "0");

	// QPicFinder
	m_closingPicfinder = false;
	LOAD_CONFIG(picfinder_refresh_msec, "1000");
	LOAD_CONFIG(ignoreImageFormatSupportWarning, "0");

#ifdef Q_WS_X11
	LOAD_CONFIG(wm, WM_UNKNOWN);
	if (m_wm == WM_UNKNOWN) {
		autoDetectWM();
	}
#endif//Q_WS_X11

	m_db = new QDBMgr(this);
	m_wallmgr = new QWallMgr(this);
	m_picfinder = new QPicFinder(this);
	m_desk = new QDesktopWidget();
}

QConfMainApp::~QConfMainApp()
{
	save();
	delete m_desk;
}

QConfMainApp& QConfMainApp::save()
{
	DECCP(QSettings, conf);

	SAVE_CONFIG(initWithoutWindow);

	// QDBMgr
	SAVE_CONFIG(pichist_max_num);

	// QWallMgr
	SAVE_CONFIG(wall_timer_sec);
	SAVE_CONFIG(max_cache_image);
	SAVE_CONFIG(disable_cache_warning);

	// QPicFinder
	SAVE_CONFIG(picfinder_refresh_msec);
	SAVE_CONFIG(ignoreImageFormatSupportWarning);

#ifdef Q_WS_X11
	SAVE_CONFIG(wm);
#endif//Q_WS_X11

	return *this;
}

#ifdef Q_WS_X11
void QConfMainApp::autoDetectWM()
{
	DECCV(WM, wm);
	bool ok;
	QFileInfoList files = QDir("/proc").entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);
	QListIterator<QFileInfo> file(files);

	while (file.hasNext() && (wm == WM_UNKNOWN)) {
		file.peekNext().fileName().toInt(&ok);

		if (ok) {
			if (QFileInfo(file.peekNext().absoluteFilePath() + "/exe").symLinkTarget().contains("kdeinit") &&
				!QFileInfo(file.peekNext().absoluteFilePath() + "/exe").symLinkTarget().contains("kdeinit4")) {
				wm = WM_KDE3;
			} else if (QFileInfo(file.peekNext().absoluteFilePath() + "/exe").symLinkTarget().contains("kdeinit4")) {
				wm = WM_KDE4;
			} else if (QFileInfo(file.peekNext().absoluteFilePath() + "/exe").symLinkTarget().contains("gconfd-2")) {
				wm = WM_GNOME;
			} else if (QFileInfo(file.peekNext().absoluteFilePath() + "/exe").symLinkTarget().contains("xfdesktop")) {
				wm = WM_XFCE;
			} else if (QFileInfo(file.peekNext().absoluteFilePath() + "/exe").symLinkTarget().contains("fluxbox")) {
				wm = WM_FLUXBOX;
			} else if (QFileInfo(file.peekNext().absoluteFilePath() + "/exe").symLinkTarget().contains("fvwm")) {
				wm = WM_FVWM;
			} else if (QFileInfo(file.peekNext().absoluteFilePath() + "/exe").symLinkTarget().contains("blackbox")) {
				wm = WM_BLACKBOX;
			} else if (QFileInfo(file.peekNext().absoluteFilePath() + "/exe").symLinkTarget().contains("wmaker")) {
				wm = WM_WINDOWMAKER;
			}
		}

		file.next();
	}

	if (wm == WM_UNKNOWN) {
		QMessageBox::critical(NULL, PROJNAME, tr("Window Manager auto detect failed"));
	}
}
#endif//Q_WS_X11
