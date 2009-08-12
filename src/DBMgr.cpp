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
	", enable INTEGER DEFAULT 1"	/* enable this dir for randpic */ \
	");" \
	""

void QDBMgr::_init()
{
	if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
		QMessageBox::critical(NULL, PROJNAME, tr("SQLITE not supported"));
		return;
	}

	DECCV(QSqlDatabase, db);
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(PROJNAME ".db");
	if (db.open()) {
		QSqlQuery(INIT_DB_PICLIST);
		QSqlQuery(INIT_DB_DIRLIST);
	} else {
		QMessageBox::critical(NULL, PROJNAME, tr("Open database failed"));
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
	sql.prepare("SELECT count(*) FROM piclist LEFT JOIN dirlist WHERE piclist.dir = dirlist.path AND enable = 1;");
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
	DECCV(QStringList, pichist);
	CDCOV(int, conf, pichist_max_num);
	QString path;
	QSqlQuery sql;
	int retry = 10;

	while (pichist.count() > pichist_max_num) {
		pichist.takeFirst();
	}

	do {
		sql.prepare("SELECT piclist.path FROM piclist LEFT JOIN dirlist WHERE piclist.dir = dirlist.path AND enable = 1 LIMIT 1 OFFSET (abs(random()) %"
			"(SELECT count(*) FROM piclist LEFT JOIN dirlist WHERE piclist.dir = dirlist.path AND enable = 1));");
		if (sql.exec()) {
			if (sql.next()) {
				path = sql.value(0).toString();
			}
		} else {
			QTRACE() << sql.lastError().text();
		}
	} while (pichist.contains(path) && (--retry > 0));

	if (retry > 0) {
		pichist.append(path);
	} else {
		if (getPicCount() < pichist_max_num)
			pichist.clear();
	}

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
	QSqlQuery("VACUUM;");
	return *this;
}

bool QDBMgr::dirEnable(const QString& path)
{
	QSqlQuery sql;
	sql.prepare("SELECT enable FROM dirlist WHERE path = ?;");
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

QDBMgr& QDBMgr::setDirEnable(const QString& path, bool enable)
{
	QSqlQuery sql;
	sql.prepare("UPDATE dirlist SET enable = ? WHERE path = ?;");
	sql.addBindValue(enable ? "1" : "0");
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
