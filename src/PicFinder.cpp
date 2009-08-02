#include "PicFinder.h"

#include "ConfMainApp.h"
#include "DBMgr.h"

#define CHECK_SUPPORTED_IMAGE_FORMAT(fmt) \
	if (list.contains(fmt)) { \
		m_dirNameFilters << "*."fmt; \
	} else { \
		if (!conf.m_ignoreImageFormatSupportWarning) { \
			if (QMessageBox::Ignore == QMessageBox::warning(&mainWidget, PROJNAME \
				, tr("System does not support image format %1").arg(QString(fmt).toUpper()) \
				, QMessageBox::Ok|QMessageBox::Ignore)) { \
				conf.m_ignoreImageFormatSupportWarning = true; \
			} \
		} \
	}


void QPicFinder::_init()
{
	DECCP(QConfMainApp, conf);
	DECOP(QWidget, conf, mainWidget);
	QList<QByteArray> list = QImageReader::supportedImageFormats();

	CHECK_SUPPORTED_IMAGE_FORMAT("bmp");
	CHECK_SUPPORTED_IMAGE_FORMAT("jpg");
	CHECK_SUPPORTED_IMAGE_FORMAT("jpeg");
	CHECK_SUPPORTED_IMAGE_FORMAT("png");
}

void QPicFinder::start(Priority priority/* = InheritPriority*/)
{
	if (isRunning())
		return;

	DECCP(QConfMainApp, conf);
	DECOV(bool, conf, closingPicfinder);
	closingPicfinder = false;
	QThread::start(priority);
}

bool QPicFinder::waitStop(unsigned long time/* = ULONG_MAX*/)
{
	DECCP(QConfMainApp, conf);
	DECOV(bool, conf, closingPicfinder);
	closingPicfinder = true;
	return QThread::wait(time);
}

void QPicFinder::addPicDir(const QString& sDir)
{
	QMutexLocker locker(&m_mutex);
	DECCP(QConfMainApp, conf);
	DECOP(QDBMgr, conf, db);

	db.addDir(sDir);
	m_dirList.append(sDir);
	start(QThread::LowestPriority);
}

void QPicFinder::removePicDir(const QString& sDir)
{
	QMutexLocker locker(&m_mutex);
	DECCP(QConfMainApp, conf);
	DECOP(QDBMgr, conf, db);
	DECCV(QStringList, dirList);
	int i;

	waitStop(10000);
	while ((i = dirList.indexOf(sDir)) >= 0) {
		dirList.removeAt(i);
	}
	db.rmDir(sDir);
	start(QThread::LowestPriority);
}

void QPicFinder::run()
{
	DECCP(QConfMainApp, conf);
	DECOP(QDBMgr, conf, db);
	CDECOV(bool, conf, closing);
	CDECOV(int, conf, picfinder_refresh_msec);
	CDECOV(bool, conf, closingPicfinder);
	DECCV(QStringList, dirNameFilters);
	DECCV(QStringList, dirList);
	QString sDir;
	int i;

	while (!dirList.isEmpty()) {
		QTime time = QTime::currentTime();
		QList<QDir> dirs;
		sDir = dirList.first();
		dirs.append(QDir(sDir));
		while (!dirs.isEmpty()) {
			QDir dir = dirs.takeFirst();

			const QFileInfoList& fiDirList = dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);
			for (i=0 ; i<fiDirList.count() ; i++) {
				if (closing || closingPicfinder)
					return;
				qApp->processEvents();

				const QFileInfo& fi = fiDirList[i];
				dirs.append(QDir(fi.absoluteFilePath()));
				QTRACE() << "DIR" << fi.absoluteFilePath();
			}

			dir.setNameFilters(dirNameFilters);
			const QFileInfoList& fiList = dir.entryInfoList(QDir::Files);
			for (i=0 ; i<fiList.count() ; i++) {
				if (closing || closingPicfinder)
					return;
				qApp->processEvents();

				const QFileInfo& fi = fiList[i];
				db.addPic(fi.absoluteFilePath(), sDir);
				if (time.msecsTo(QTime::currentTime()) > picfinder_refresh_msec) {
					emit(dirFoundSome(sDir));
					time = QTime::currentTime();
				}
				QTRACE() << fi.absoluteFilePath();
			}
		}

		dirList.removeFirst();
		emit(dirFoundOver(sDir));
	}
}
