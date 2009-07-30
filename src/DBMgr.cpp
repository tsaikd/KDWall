#include "DBMgr.h"

#include "ConfMainApp.h"

#define INIT_DB_PICLIST \
	"CREATE TABLE IF NOT EXISTS piclist " \
	"( path TEXT PRIMARY KEY"		/* picture path */ \
	", dir TEXT"					/* picture dir, used for search */ \
	", width INTEGER DEFAULT 0"		/* picture width */ \
	", height INTEGER DEFAULT 0"	/* picture height */ \
	");" \
	"CREATE INDEX IF NOT EXISTS piclist_path ON piclist ( path );" \
	""

#define INIT_DB_DIRLIST \
	"CREATE TABLE IF NOT EXISTS dirlist " \
	"( path TEXT PRIMARY KEY"		/* dir path */ \
	");" \
	""

void QDBMgr::_init()
{
	DECCV(QSqlDatabase, db);
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(PROJNAME ".db");
	if (db.open()) {
		QSqlQuery(INIT_DB_PICLIST);
		QSqlQuery(INIT_DB_DIRLIST);
	} else {
		QMessageBox(QMessageBox::Critical, PROJNAME, tr("Open database failed"));
	}

}

QDBMgr& QDBMgr::addPic(const QString& path, const QString& dir, int width/* = 0*/, int height/* = 0*/)
{
	QSqlQuery sql;
	sql.prepare("INSERT INTO piclist ( path, dir, width, height ) VALUES ( ? , ? , ? , ? );");
	sql.addBindValue(path);
	sql.addBindValue(dir);
	sql.addBindValue(width);
	sql.addBindValue(height);
	if (!sql.exec()) {
//		QTRACE() << sql.lastError().text();
	}

	return *this;
}

int QDBMgr::getPicCount()
{
	QSqlQuery sql;
	sql.prepare("SELECT count(*) FROM piclist;");
	if (sql.exec()) {
		if (sql.next()) {
			return sql.value(0).toInt();
		}
	} else {
		QTRACE() << sql.lastError().text();
	}
	return 0;
}

QString QDBMgr::getRandPic()
{
	DECCP(QConfMainApp, conf);
	DECCV(QList<int>, pichist);
	QString path;
	QSqlQuery sql;
	int count = getPicCount();
	int num = qrand() % count;

	while (pichist.count() > conf.m_pichist_max_num) {
		pichist.takeFirst();
	}
	if (count > (pichist.count()<<1)) {
		while (pichist.contains(num)) {
			num = qrand() % count;
		}
	} else {
		pichist.clear();
	}

	sql.prepare("SELECT path FROM piclist LIMIT 1 OFFSET ?;");
	sql.addBindValue(num);
	if (sql.exec()) {
		if (sql.next()) {
			path = sql.value(0).toString();
		}
	} else {
		QTRACE() << sql.lastError().text();
	}
	pichist.append(num);
	return path;
}

QDBMgr& QDBMgr::addDir(const QString& path)
{
	QSqlQuery sql;
	sql.prepare("INSERT INTO dirlist ( path ) VALUES ( ? );");
	sql.addBindValue(path);
	if (!sql.exec()) {
//		QTRACE() << sql.lastError().text();
	}

	return *this;
}

QDBMgr& QDBMgr::rmDir(const QString& path)
{
	QSqlQuery sql;
	sql.prepare("DELETE FROM dirlist WHERE path = ?;");
	sql.addBindValue(path);
	if (!sql.exec()) {
		QTRACE() << sql.lastError().text();
	}
	sql.prepare("DELETE FROM piclist WHERE dir = ?;");
	sql.addBindValue(path);
	if (!sql.exec()) {
		QTRACE() << sql.lastError().text();
	}
	return *this;
}

bool QDBMgr::isDirExists(const QString& path)
{
	QSqlQuery sql;
	sql.prepare("SELECT count(*) FROM dirlist WHERE path = ?;");
	sql.addBindValue(path);
	if (sql.exec()) {
		if (sql.next()) {
			if (sql.value(0).toInt() > 0) {
				return true;
			}
		}
	} else {
		QTRACE() << sql.lastError().text();
	}
	return false;
}

int QDBMgr::getDirPicCount(const QString& path)
{
	QSqlQuery sql;
	sql.prepare("SELECT count(*) FROM piclist WHERE dir = ?;");
	sql.addBindValue(path);
	if (sql.exec()) {
		if (sql.next()) {
			return sql.value(0).toInt();
		}
	} else {
		QTRACE() << sql.lastError().text();
	}
	return 0;
}

QStringList QDBMgr::getDirList()
{
	QStringList res;
	QSqlQuery sql;
	sql.prepare("SELECT path FROM dirlist;");
	if (sql.exec()) {
		while (sql.next()) {
			res << sql.value(0).toString();
		}
	} else {
		QTRACE() << sql.lastError().text();
	}

	return res;
}

QDBMgr& QDBMgr::clearDB()
{
	QSqlQuery("DELETE FROM piclist; DELETE FROM dirlist;");
	return *this;
}
