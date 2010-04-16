#include "ConfMainApp.h"
#include "ExtConfCppInc.h"

#include "DBMgr.h"
#include "WallMgr.h"
#include "PicFinder.h"

#ifdef Q_WS_X11
static const QVariant& operator << (WM& val, const QVariant& var) {
	val = (WM)var.toInt();
	return var;
}
#endif//Q_WS_X11

QConfMainApp::~QConfMainApp()
{
	save();
	delete m_desk;
}

void QConfMainApp::load()
{
	m_mainWidget = NULL;
	m_closing = false;
	LOAD_CONFIG(initWithoutWindow, "0");

	// QDBMgr
	LOAD_CONFIG(pichist_max_num, "100");

	// QWallMgr
	LOAD_CONFIG(wall_timer_sec, "5");
	LOAD_CONFIG(max_cache_image, "5");
	LOAD_CONFIG(disable_cache_warning, "0");
	LOAD_CONFIG(still_run_when_fullscreen, "0");

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

void QConfMainApp::save()
{
	SAVE_CONFIG(initWithoutWindow);

	// QDBMgr
	SAVE_CONFIG(pichist_max_num);

	// QWallMgr
	SAVE_CONFIG(wall_timer_sec);
	SAVE_CONFIG(max_cache_image);
	SAVE_CONFIG(disable_cache_warning);
	SAVE_CONFIG(still_run_when_fullscreen);

	// QPicFinder
	SAVE_CONFIG(picfinder_refresh_msec);
	SAVE_CONFIG(ignoreImageFormatSupportWarning);

#ifdef Q_WS_X11
	SAVE_CONFIG(wm);
#endif//Q_WS_X11

	Super::save();
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
