#include "WallMgr.h"

#include "Util.h"
#include "ConfMainApp.h"
#include "DBMgr.h"

#ifdef Q_WS_WIN
	#include <windows.h>
#endif

void QWallPaperParam::_init()
{
	m_style = STYLE_EXPAND;
	m_useOrigin = false;
}

QWallPaperParam& QWallPaperParam::close()
{
	if (!m_useOrigin && !m_tmpPath.isEmpty()) {
		if (!QFile::remove(m_tmpPath)) {
			QTRACE("Remove file" << m_tmpPath << "failed");
		}
	}

	m_path.clear();
	m_tmpPath.clear();
	_init();
	return *this;
}

bool QWallPaperParam::operator == (const QWallPaperParam& x) const
{
	if (m_style != x.m_style)
		return false;
	if (m_path != x.m_path)
		return false;
	return true;
}

void QWallCacheMaker::_init()
{
}

QWallCacheMaker::~QWallCacheMaker()
{
	DECCV(QWallParamList, beMadeWallList);
	while (!beMadeWallList.isEmpty()) {
		delete beMadeWallList.takeFirst();
	}
}

QWallCacheMaker& QWallCacheMaker::addWall(QWallPaperParam* wall)
{
	{
		QMutexLocker locker(&m_beMadeWallMutex);
		m_beMadeWallList.append(wall);
	}

	if (!isRunning())
		start(QThread::LowestPriority);

	return *this;
}

/**
 * @brief fill wall.m_tmpPath, create image file if need
 **/
bool QWallCacheMaker::prepareCacheImg(QWallPaperParam& wall)
{
	DECCP(QConfMainApp, conf);
	CDCOP(QDesktopWidget, conf, desk);
	DECOV(bool, conf, disable_cache_warning);
	DECOV(QWidget*, conf, mainWidget);
	CDCOV(QString, wall, path);
	DECOV(QString, wall, tmpPath);
	DECOV(bool, wall, useOrigin);
	QImage img;

#if defined(Q_WS_X11)
	// ubuntu support set wallpaper with jpeg format
	useOrigin = true;
#endif//defined(Q_WS_X11)

	if (useOrigin) {
		tmpPath = path;
	} else {
		QTemporaryFile tmp(QDir::tempPath()+"/"PROJNAME"_XXXXXX.bmp");
		tmp.setAutoRemove(false);
		if (!tmp.open()) {
			QMSGBOX_WARN_CONTROL(disable_cache_warning, mainWidget, PROJNAME, tr("Create tmp file failed"));
			return false;
		}

		QRect rcDesk = desk.geometry();
		switch (conf.m_wall_resize_policy) {
		case WALL_RESIZE_POLICY_MAX_SCREEN:
			for (int i=0 ; i<desk.numScreens() ; i++) {
				const QRect& rc = desk.screenGeometry(i);
				if (rc.width() * rc.height() > rcDesk.width() * rcDesk.height()) {
					rcDesk = rc;
				}
			}
			break;
		case WALL_RESIZE_POLICY_MIN_SCREEN:
			for (int i=0 ; i<desk.numScreens() ; i++) {
				const QRect& rc = desk.screenGeometry(i);
				if (rc.width() * rc.height() < rcDesk.width() * rcDesk.height()) {
					rcDesk = rc;
				}
			}
			break;
		default:
			break;
		}

		tmpPath = QDir::toNativeSeparators(tmp.fileName());
		if (!img.load(path)) {
			QMSGBOX_WARN_CONTROL(disable_cache_warning, mainWidget, PROJNAME, tr("Load image failed ('%1')").arg(path));
			return false;
		}
		if (img.height() > rcDesk.height()) {
			img = img.scaledToHeight(rcDesk.height(), Qt::SmoothTransformation);
		}
		if (img.width() > rcDesk.width()) {
			img = img.scaledToWidth(rcDesk.width(), Qt::SmoothTransformation);
		}
		if (!img.save(tmpPath, "BMP")) {
			QMSGBOX_WARN_CONTROL(disable_cache_warning, mainWidget, PROJNAME, tr("Save image to BMP failed ('%1')").arg(tmpPath));
			return false;
		}
	}

	return true;
}

int QWallCacheMaker::wallListCount()
{
	QMutexLocker locker1(&m_beMadeWallMutex);
	QMutexLocker locker2(m_wallMutex);

	return m_beMadeWallList.count() + m_wallList->count();
}

void QWallCacheMaker::run()
{
	CDCCP(QConfMainApp, conf);
	DECCV(QWallParamList, beMadeWallList);
	DECCP(QWallParamList, wallList);
	QWallPaperParam* wall;
	QImage img;

	while (!beMadeWallList.isEmpty() && !conf.m_closing) {
		// fill wall.m_tmpPath, create image file if need
		{
			QMutexLocker locker(&m_beMadeWallMutex);
			wall = beMadeWallList.takeFirst();;
			QTRACE("beMadeWallList:" << beMadeWallList.count());
		}
		if (!prepareCacheImg(*wall))
			continue;
		{
			QMutexLocker locker(m_wallMutex);
			wallList.append(wall);
			QTRACE("wallList:" << wallList.count());
		}
	}
}

void QWallMgr::_init()
{
	DECCP(QConfMainApp, conf);
	CDCOV(int, conf, wall_timer_sec);

	m_cacheMaker = new QWallCacheMaker(conf, m_wallList, &m_mutex, parent());

	getWallPaper(m_initWall);
	m_initWall.m_useOrigin = true;
	m_timerId = startTimer(wall_timer_sec * 1000);
}

QWallMgr::~QWallMgr()
{
	DECCV(QWallPaperParam, initWall);
	DECCV(QWallParamList, wallList);
	QWallPaperParam wall;

	getWallPaper(wall);
	if (wall != initWall) {
		setWallPaper(initWall);
	}

	while (!wallList.isEmpty()) {
		delete wallList.takeFirst();
	}
}

bool QWallMgr::getWallPaper(QWallPaperParam& wall)
{
	QMutexLocker locker(&m_mutex);
	wall.close();

#if defined(Q_WS_WIN)
	QSettings reg("HKEY_CURRENT_USER\\Control Panel\\Desktop", QSettings::NativeFormat);

	wall.m_path = reg.value("Wallpaper").toString();
	int style = reg.value("WallpaperStyle").toInt();
	int tile = reg.value("TileWallpaper").toInt();
	if (style == 0) {
		if (tile == 0) {
			wall.m_style = QWallPaperParam::STYLE_CENTER;
		} else {
			wall.m_style = QWallPaperParam::STYLE_TILE;
		}
	} else {
		// style == 2
		wall.m_style = QWallPaperParam::STYLE_EXPAND;
	}
	return true;
#elif defined(Q_WS_X11)
	DECCP(QConfMainApp, conf);
	QStringList args;
	QString procout;
	switch (conf.m_wm) {
	case WM_GNOME:
		{
			QProcess proc;
			proc.setProcessChannelMode(QProcess::MergedChannels);
			args.clear();
			args << "--get" << "/desktop/gnome/background/picture_filename";
			proc.start("gconftool-2", args);
			if (proc.waitForFinished()) {
				procout = proc.readAll();
				procout = procout.trimmed();
				wall.m_path = procout;
			} else {
				QTRACE(proc.errorString());
				return false;
			}
		}
		{
			QProcess proc;
			proc.setProcessChannelMode(QProcess::MergedChannels);
			args.clear();
			args << "--get" << "/desktop/gnome/background/picture_options";
			proc.start("gconftool-2", args);
			if (proc.waitForFinished()) {
				procout = proc.readAll();
				procout = procout.trimmed();
				if (procout == "centered") {
					wall.m_style = QWallPaperParam::STYLE_CENTER;
				} else {
					DNOTIMP();
					return false;
				}
			} else {
				QTRACE(proc.errorString());
				return false;
			}
		}
		return true;
	default:
		break;
	}
#endif//Q_WS_*

	return false;
}

#define RETURN(x) { emit(changedWallPaper(wall)); return (x); }
bool QWallMgr::setWallPaper(QWallPaperParam& wall)
{
	QMutexLocker locker(&m_mutex);
	CDCOV(QString, wall, path);
	CDCOV(QWallPaperParam::STYLE, wall, style);
	DECOV(QString, wall, tmpPath);
	DECOV(bool, wall, useOrigin);
	QImage img;

	emit(changingWallPaper(wall));

	if (tmpPath.isEmpty()) {
		tmpPath = path;
		useOrigin = true;
	}

#if defined(Q_WS_WIN)
	{	// share desktop will clean wallpaper setting, consider the situation
		QWallPaperParam cur;
		if (!getWallPaper(cur))
			RETURN(false);
		if (cur.m_path.isEmpty())
			RETURN(false);
	}

	QSettings reg("HKEY_CURRENT_USER\\Control Panel\\Desktop", QSettings::NativeFormat);
	reg.setValue("Wallpaper", tmpPath);
	if (style == QWallPaperParam::STYLE_EXPAND) {
		reg.setValue("WallpaperStyle", "2");
		reg.setValue("TileWallpaper", "0");
	} else if (style == QWallPaperParam::STYLE_CENTER) {
		reg.setValue("WallpaperStyle", "0");
		reg.setValue("TileWallpaper", "0");
	} else {
		reg.setValue("WallpaperStyle", "0");
		reg.setValue("TileWallpaper", "1");
	}
	SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0
		, (PVOID)(tmpPath.toAscii().data())
		, SPIF_SENDWININICHANGE);

	emit(changedWallPaper(wall));

	RETURN(true);
#elif defined(Q_WS_X11)
	DECCP(QConfMainApp, conf);
	QStringList args;
	QFile textFile;
	QTextStream textStream;
	switch (conf.m_wm) {
	case WM_KDE3:
		args << "kdesktop" << "KBackgroundIface" << "setWallpaper"
			<< tmpPath << "1";
		QProcess::execute("dcop", args);
		RETURN(true);
	case WM_KDE4:
		textFile.setFileName(QDir::homePath() + "/.wally/wallykde4.img");
		textFile.open(QIODevice::WriteOnly);
		textStream.setDevice(&textFile);

		textStream << tmpPath << endl;
		textFile.close();
		RETURN(true);
	case WM_GNOME:
		args << "--type" << "bool" << "--set" <<
			"/desktop/gnome/background/draw_background" << "true";
		QProcess::execute("gconftool-2", args);
		args.clear();
		args << "--type" << "string" << "--set" <<
			"/desktop/gnome/background/picture_options" << "centered";
		QProcess::execute("gconftool-2", args);
		args.clear();
		args << "--type" << "string" << "--set" <<
			"/desktop/gnome/background/picture_filename" << tmpPath;
		QProcess::execute("gconftool-2", args);
		RETURN(true);
	case WM_XFCE:
		textFile.setFileName(QDir::homePath() + "/.config/xfce4/desktop/backdrops.list");
		textFile.open(QIODevice::WriteOnly);
		textStream.setDevice(&textFile);

		textStream << "# xfce backdrop list" << endl;
		textStream << tmpPath << endl;
		textFile.close();

		args << "--reload";
		QProcess::execute("xfdesktop", args);
		RETURN(true);
	case WM_FLUXBOX:
		args << "-c" << tmpPath;
		QProcess::execute("fbsetbg", args);
		RETURN(true);
	case WM_FVWM:
		args << tmpPath;
		QProcess::execute("fvwm-root", args);
		RETURN(true);
	case WM_BLACKBOX:
		args << "-center" << tmpPath;
		QProcess::execute("bsetbg", args);
		RETURN(true);
	case WM_WINDOWMAKER:
		args << "-e" << tmpPath;
		QProcess::execute("wmsetbg",args);
		RETURN(true);
	default:
		RETURN(false);
	}
#endif//Q_WS_*

	RETURN(false);
}
#undef RETURN

bool QWallMgr::setRandWallPaper()
{
	QMutexLocker locker(&m_mutex);
	DECCP(QConfMainApp, conf);
	DECOP(QDBMgr, conf, db);
	DECOV(int, conf, wall_timer_sec);
	DECCV(int, timerId);
	DECCP(QWallCacheMaker, cacheMaker);
	DECCV(QWallParamList, wallList);
	CDCOV(int, conf, max_cache_image);
	QWallPaperParam* wall;
	QString path;
	int retry;

	if (!conf.m_still_run_when_fullscreen && !QUtil::isDesktopVisible())
		return false;

	if (timerId) {
		killTimer(timerId);
		timerId = 0;
	}

	if (db.getPicCount() <= 0)
		return false;

	if (wallList.isEmpty()) {
		wall = new QWallPaperParam();
		retry = 10;
		do {
			wall->m_path = db.getRandPic();
			wall->m_style = QWallPaperParam::STYLE_CENTER;
		} while (!cacheMaker.prepareCacheImg(*wall) && (retry-- > 0));
		if (retry <= 0)
			return false;
	} else {
		wall = wallList.takeFirst();
	}

	bool bRes = setWallPaper(*wall);
	SAFE_DELETE(wall);

	timerId = startTimer(wall_timer_sec * 1000);

	retry = max_cache_image - cacheMaker.wallListCount();
	while (retry-- > 0) {
		wall = new QWallPaperParam();
		wall->m_path = db.getRandPic();
		wall->m_style = QWallPaperParam::STYLE_CENTER;
		cacheMaker.addWall(wall);
	}

	return bRes;
}

void QWallMgr::clearCacheWall()
{
	QMutexLocker locker(&m_mutex);
	DECCV(QWallParamList, wallList);
	wallList.clear();
}

void QWallMgr::timerEvent(QTimerEvent* e)
{
	DECCV(int, timerId);
	if (timerId == e->timerId()) {
		setRandWallPaper();
	} else {
		killTimer(e->timerId());
	}
}
