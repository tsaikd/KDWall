#include "ConfMainApp.h"

#include "DBMgr.h"
#include "WallMgr.h"
#include "PicFinder.h"

#define LOAD_CONFIG(var, defvalue) m_##var << conf.value(#var, defvalue); m_mapConfDefValue.insert(#var, defvalue);
#define SAVE_CONFIG(var) if (m_##var == m_mapConfDefValue.value(#var)) { conf.remove(#var); } else { conf.setValue(#var, m_##var); }

static const QVariant& operator << (bool val, const QVariant& var) {
	val = var.toBool();
	return var;
}

static const QVariant& operator << (int val, const QVariant& var) {
	val = var.toInt();
	return var;
}

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

	m_ignoreImageFormatSupportWarning << m_conf->value("A", "B");

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

	return *this;
}
