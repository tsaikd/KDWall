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
	QConfMainApp& save();

	QSettings* m_conf;
	QWidget* m_mainWidget;
	bool	m_closing;
	bool	m_initWithoutWindow;					// ini conf

	QDBMgr* m_db;
	int		m_pichist_max_num;						// ini conf

	QWallMgr* m_wallmgr;
	int		m_wall_timer_sec;						// ini conf
	int		m_max_cache_image;						// ini conf
	bool	m_disable_cache_warning;				// ini conf

	QPicFinder* m_picfinder;
	bool	m_closingPicfinder;
	bool	m_ignoreImageFormatSupportWarning;		// ini conf
	int		m_picfinder_refresh_msec;				// ini conf

	QDesktopWidget* m_desk;

protected:
	QMap<QString, QVariant> m_mapConfDefValue;
};

#endif//_KDWALL_QCONFMAINAPP_H
