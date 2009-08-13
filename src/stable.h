#include <QtCore>
#include <QtGui>
#include <QtSql>

#define PROJNAME "KDWall"

#include "Debug.h"

#define QMSGBOX_WARN_CONTROL(isDisable, parent, title, msg) \
	if (!isDisable) { \
		if (QMessageBox::Ignore == QMessageBox::warning(parent, title, msg, QMessageBox::Ok|QMessageBox::Ignore)) { \
			isDisable = true; \
		} \
	}
