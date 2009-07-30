#ifndef _KDWALL_PICFINDER_H
#define _KDWALL_PICFINDER_H

#include "stable.h"

class QConfMainApp;

class QPicFinder : public QThread
{
	Q_OBJECT
private:
	void _init();
public:
	QPicFinder(QConfMainApp* conf)
		:	m_conf(conf), QThread((QObject*)conf), m_mutex(QMutex::Recursive)
	{ _init(); }

	void addPicDir(const QString& sDir);

protected:
	virtual void run() { exec(); }
	void findPicInDir();
	void addPicToDB(const QFileInfo& fi, const QString& sDir);

signals:
	void findStepOne();
	void dirFoundSome(const QString& sDir);
	void dirFoundOver(const QString& sDir);

protected:
	QConfMainApp* m_conf;
	QMutex m_mutex;
	QStringList m_dirNameFilters;
	QStringList m_dirList;
	int m_picFoundCount;
};

#endif//_KDWALL_PICFINDER_H
