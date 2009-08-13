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
		:	QThread((QObject*)conf), m_conf(conf), m_mutex(QMutex::Recursive)
	{ _init(); }

public slots: // rewrite functions
	void start(Priority priority = InheritPriority);

	bool waitStop(unsigned long time = ULONG_MAX);
	void addPicDir(const QString& sDir);
	void removePicDir(const QString& sDir);

protected:
	virtual void run();
	void findPicInDir();

signals:
	void findStepOne();
	void dirFoundSome(const QString& sDir);
	void dirFoundOver(const QString& sDir);

protected:
	QConfMainApp* m_conf;
	QMutex m_mutex;
	QStringList m_dirNameFilters;
	QStringList m_dirList;
};

#endif//_KDWALL_PICFINDER_H
