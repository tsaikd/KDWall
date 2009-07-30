#include "WallMgr.h"

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
	m_path.clear();
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

void QWallMgr::_init()
{
	getWallPaper(m_initWall);
	m_initWall.m_useOrigin = true;
	m_timerId = 0;
}

bool QWallMgr::getWallPaper(QWallPaperParam& wall)
{
	QMutexLocker locker(&m_mutex);
	typedef QWallPaperParam::STYLE STYLE;
#ifdef Q_WS_WIN
	QSettings reg("HKEY_CURRENT_USER\\Control Panel\\Desktop", QSettings::NativeFormat);

	wall.close();
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
#endif//Q_WS_WIN

	return true;
}

bool QWallMgr::setWallPaper(const QWallPaperParam& wall)
{
	QMutexLocker locker(&m_mutex);
	CDECCP(QConfMainApp, conf);
	CDECOP(QDesktopWidget, conf, desk);
	CDECOV(QString, wall, path);
	CDECOV(QWallPaperParam::STYLE, wall, style);

#ifdef Q_WS_WIN
	// share desktop will clean wallpaper setting, consider the situation
	QWallPaperParam cur;
	if (!getWallPaper(cur))
		return false;
	if (cur.m_path.isEmpty())
		return false;

	QString tmpPath = path;
	if (!wall.m_useOrigin) {
		QTemporaryFile tmp(QDir::tempPath()+"/zzXXXXXX.bmp");
		tmp.setAutoRemove(false);
		if (!tmp.open())
			return false;
		tmpPath = QDir::toNativeSeparators(tmp.fileName());
		QImage img(path);
		if (img.height() > desk.height()) {
			img = img.scaledToHeight(desk.height(), Qt::SmoothTransformation);
		}
		if (img.width() > desk.width()) {
			img = img.scaledToWidth(desk.width(), Qt::SmoothTransformation);
		}
		img.save(tmpPath, "BMP");
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
	QTRACE() << path;

	if (!wall.m_useOrigin) {
		QFile::remove(tmpPath);
	}

	return true;
#endif//Q_WS_WIN

	return false;
}

bool QWallMgr::setRandWallPaper()
{
	QMutexLocker locker(&m_mutex);
	DECCP(QConfMainApp, conf);
	DECOP(QDBMgr, conf, db);
	DECOV(int, conf, wall_timer_sec);
	DECCV(int, timerId);

	if (timerId) {
		killTimer(timerId);
		timerId = 0;
	}

	if (db.getPicCount() <= 0)
		return false;

	QWallPaperParam wall;
	wall.m_path = db.getRandPic();
	wall.m_style = QWallPaperParam::STYLE_CENTER;
	bool bRes = setWallPaper(wall);

	timerId = startTimer(wall_timer_sec * 1000);

	return bRes;
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
