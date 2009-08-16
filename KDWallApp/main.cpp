#include "stable.h"

#include "config.h"
#include "QtSingleApplication"
#include "WinMainApp.h"

int main(int argc, char* argv[])
{
	QtSingleApplication app(PROJNAME"App", argc, argv);
	if (app.sendMessage(QString::number(APPMSG_SHOWGUI)))
		return 0;

	qsrand(QDateTime::currentDateTime().toTime_t());
	app.setApplicationName(qAppName());
	app.setApplicationVersion(PROJVER);

	QTranslator lang;
	lang.load(QString(":/lang/%1_%2.qm").arg(PROJNAME).arg(QLocale::system().name()));
	app.installTranslator(&lang);

	QWinMainApp win;
	QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &win, SLOT(handleAppMessage(const QString&)));

	return app.exec();
}
