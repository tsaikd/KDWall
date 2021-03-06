#ifndef _KDWALL_DBMGR_H
#define _KDWALL_DBMGR_H

#include "stable.h"

class QConfMainApp;

class QDBMgr : public QObject
{
private:
	void _init();
public:
	QDBMgr(QConfMainApp* conf)
		:	QObject((QObject*)conf), m_conf(conf)
	{ _init(); }

	QDBMgr& addPic(const QString& path, const QString& dir, int width = 0, int height = 0);
	int getPicCount();
	QString getRandPic();
	QDBMgr& addDir(const QString& path);
	QDBMgr& rmDir(const QString& path);
	bool dirEnable(const QString& path);
	QDBMgr& setDirEnable(const QString& path, bool enable);
	bool isDirExists(const QString& path);
	int getDirPicCount(const QString& path);
	QStringList getDirList();
	QDBMgr& clearDB();

protected:
	QConfMainApp* m_conf;
	QSqlDatabase m_db;
	QStringList m_pichist;
};

#endif//_KDWALL_DBMGR_H
