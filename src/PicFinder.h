#ifndef _KDWALL_PICFINDER_H
#define _KDWALL_PICFINDER_H

#include "stable.h"

class QWallPaperParam;

class QPicFinder : public QThread
{
	Q_OBJECT
private:
	void _init();
public:
	QPicFinder(QObject* parent = NULL)
		:	QThread(parent), m_mutex(QMutex::Recursive)
	{ _init(); }

	void addPicDir(const QString& sDir);
	void clearDB();

protected:
	virtual void run() { exec(); }
	void findPicInDir();
	void addPicToDB(const QFileInfo& fi, const QString& sDir);

signals:
	void findStepOne();
	void dirFoundOver(const QString& sDir);

protected:
	QMutex m_mutex;
	QSqlDatabase m_sqlDB;
	QStringList m_dirNameFilters;
	QStringList m_dirList;
	int m_picFoundCount;
};

#endif//_KDWALL_PICFINDER_H
