#include "PicFinder.h"

#define INIT_DB_PICLIST \
	"CREATE TABLE IF NOT EXISTS piclist " \
	"( path TEXT PRIMARY KEY"		/* picture path */ \
	", dir TEXT"					/* picture dir, used for search */ \
	", width INTEGER DEFAULT 0"		/* picture width */ \
	", height INTEGER DEFAULT 0"	/* picture height */ \
	");" \
	"CREATE INDEX IF NOT EXISTS piclist_path ON piclist ( path );" \
	""

void QPicFinder::_init()
{
	QSqlDatabase& sqlDB = m_sqlDB;
	sqlDB = QSqlDatabase::addDatabase("QSQLITE");
	sqlDB.setDatabaseName(PROJNAME ".db");
	if (sqlDB.open()) {
		QSqlQuery(INIT_DB_PICLIST);
	} else {
		QMessageBox(QMessageBox::Critical, PROJNAME, tr("Open database failed"));
	}

	m_dirNameFilters << "*.bmp";
	m_dirNameFilters << "*.jpg";
	m_dirNameFilters << "*.jpeg";
	m_dirNameFilters << "*.png";

	m_picFoundCount = 0;
}

void QPicFinder::addPicDir(const QString& sDir)
{
	QMutexLocker locker(&m_mutex);
	m_dirList.append(sDir);
	findPicInDir();
}

void QPicFinder::clearDB()
{
	QSqlQuery("DELETE FROM piclist;");
}

void QPicFinder::findPicInDir()
{
	QMutexLocker locker(&m_mutex);
	QStringList& dirNameFilters = m_dirNameFilters;
	QStringList& dirList = m_dirList;
	int i;

	while (!dirList.isEmpty()) {
		QList<QDir> dirs;
		const QString& sDir = dirList.takeFirst();
		dirs.append(QDir(sDir));
		while (!dirs.isEmpty()) {
			QDir& dir = dirs.takeFirst();

			const QFileInfoList& fiDirList = dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);
			for (i=0 ; i<fiDirList.count() ; i++) {
				const QFileInfo& fi = fiDirList[i];
				dirs.append(QDir(fi.absoluteFilePath()));
				QTRACE() << "DIR" << fi.absoluteFilePath();
			}

			dir.setNameFilters(dirNameFilters);
			const QFileInfoList& fiList = dir.entryInfoList(QDir::Files);
			for (i=0 ; i<fiList.count() ; i++) {
				const QFileInfo& fi = fiList[i];
				addPicToDB(fi, sDir);
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

	QSqlQuery sql;
	sql.prepare("INSERT INTO piclist ( path, dir, width, height ) VALUES ( ? , ? , ? , ? );");
	sql.addBindValue(fi.absoluteFilePath());
	sql.addBindValue(sDir);
	sql.addBindValue(0);
	sql.addBindValue(0);
	if (!sql.exec()) {
		QTRACE() << sql.lastError().text();
	}
}
