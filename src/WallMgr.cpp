#include "WallMgr.h"

#ifdef Q_WS_WIN
	#include <windows.h>
#endif

void QWallPaperParam::_init()
{
	m_style = STYLE_EXPAND;
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
}

QWallMgr& QWallMgr::close()
{
	m_listWall.clear();

	_init();
	return *this;
}

bool QWallMgr::getWallPaper(QWallPaperParam& wall)
{
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
	const QString& path = wall.m_path;
	const QWallPaperParam::STYLE& style = wall.m_style;

	m_listWall.push_front(wall);

#ifdef Q_WS_WIN
	QSettings reg("HKEY_CURRENT_USER\\Control Panel\\Desktop", QSettings::NativeFormat);
	reg.setValue("Wallpaper", QDir::toNativeSeparators(path));
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
	SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, reinterpret_cast<void *>
		(QDir::toNativeSeparators(path).toAscii().data()),
		SPIF_SENDWININICHANGE);

	return true;
#endif//Q_WS_WIN

	return false;
}
