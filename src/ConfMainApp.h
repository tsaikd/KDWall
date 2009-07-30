#ifndef _KDWALL_QCONFMAINAPP_H
#define _KDWALL_QCONFMAINAPP_H

#include "stable.h"

class QWallMgr;
class QPicFinder;
class QDBMgr;

class QConfMainApp : public QObject
{
private:
	void _init();
public:
	QConfMainApp(QObject* parent = NULL)
		:	QObject(parent)
	{ _init(); }
	~QConfMainApp();

	QSettings* m_conf;
	bool m_closing;

	QDBMgr* m_db;
	int m_pichist_max_num;

	QWallMgr* m_wallmgr;
	int m_wall_timer_sec;

	QPicFinder* m_picfinder;
	int m_picfinder_refresh_msec;

	QDesktopWidget* m_desk;
};

#endif//_KDWALL_QCONFMAINAPP_H
