#include "mainApp.h"

#include "QtSingleApplication"
#include "WinMainApp.h"

int main(int argc, char* argv[])
{
	QtSingleApplication app(PROJNAME"App", argc, argv);
	if (app.sendMessage(QString::number(APPMSG_SHOWGUI)))
		return 0;

	QTranslator lang;
	lang.load(QString(":/lang/"PROJNAME"_%1.qm").arg(QLocale::system().name()));
	app.installTranslator(&lang);

	QWinMainApp win;
	QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &win, SLOT(handleAppMessage(const QString&)));

	return app.exec();
}
