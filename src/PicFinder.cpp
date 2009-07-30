#include "PicFinder.h"

#include "ConfMainApp.h"
#include "DBMgr.h"

void QPicFinder::_init()
{
	m_dirNameFilters << "*.bmp";
	m_dirNameFilters << "*.jpg";
	m_dirNameFilters << "*.jpeg";
	m_dirNameFilters << "*.png";

	m_picFoundCount = 0;
}

void QPicFinder::addPicDir(const QString& sDir)
{
	QMutexLocker locker(&m_mutex);
	DECCP(QConfMainApp, conf);
	DECOP(QDBMgr, conf, db);

	db.addDir(sDir);
	m_dirList.append(sDir);
	findPicInDir();
}

void QPicFinder::findPicInDir()
{
	QMutexLocker locker(&m_mutex);
	DECCP(QConfMainApp, conf);
	QStringList& dirNameFilters = m_dirNameFilters;
	QStringList& dirList = m_dirList;
	int i;

	while (!dirList.isEmpty()) {
		QTime time = QTime::currentTime();
		QList<QDir> dirs;
		const QString& sDir = dirList.takeFirst();
		dirs.append(QDir(sDir));
		while (!dirs.isEmpty()) {
			QDir& dir = dirs.takeFirst();

			const QFileInfoList& fiDirList = dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);
			for (i=0 ; i<fiDirList.count() ; i++) {
				if (conf.m_closing)
					return;

				const QFileInfo& fi = fiDirList[i];
				dirs.append(QDir(fi.absoluteFilePath()));
				QTRACE() << "DIR" << fi.absoluteFilePath();
			}

			dir.setNameFilters(dirNameFilters);
			const QFileInfoList& fiList = dir.entryInfoList(QDir::Files);
			for (i=0 ; i<fiList.count() ; i++) {
				if (conf.m_closing)
					return;
msleep(500);
				const QFileInfo& fi = fiList[i];
				addPicToDB(fi, sDir);
				if (time.msecsTo(QTime::currentTime()) > conf.m_picfinder_refresh_msec) {
					emit(dirFoundSome(sDir));
					time = QTime::currentTime();
				}
				QTRACE() << fi.absoluteFilePath();
			}
		}

		emit(dirFoundOver(sDir));
	}
}

void QPicFinder::addPicToDB(const QFileInfo& fi, const QString& sDir)
{
	if (m_picFoundCount++ > 100) {
		emit(findStepOne());
	}

	DECCP(QConfMainApp, conf);
	DECOP(QDBMgr, conf, db);
	db.addPic(fi.absoluteFilePath(), sDir);
}
