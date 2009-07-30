#include "ConfMainApp.h"

#include "DBMgr.h"
#include "WallMgr.h"
#include "PicFinder.h"

void QConfMainApp::_init()
{
	m_conf = new QSettings(PROJNAME".ini", QSettings::IniFormat, this);
	DECCP(QSettings, conf);
	m_closing = false;

	m_db = new QDBMgr(this);
	m_pichist_max_num = conf.value("pichist_max_num", "100").toInt();

	m_wallmgr = new QWallMgr(this);
	m_wall_timer_sec = conf.value("wall_timer_sec", "5").toInt();

	m_picfinder = new QPicFinder(this);
	m_picfinder_refresh_msec = conf.value("picfinder_refresh_msec", "1000").toInt();

	m_desk = new QDesktopWidget();

	qsrand(QDateTime::currentDateTime().toTime_t());
}

QConfMainApp::~QConfMainApp()
{
	delete m_desk;
}
