#include "mainApp.h"

#include "QtSingleApplication"
#include "WinMainApp.h"

int main(int argc, char* argv[])
{
	QtSingleApplication app(PROJNAME"App", argc, argv);
	if (app.sendMessage(QString::number(APPMSG_SHOWGUI)))
		return 0;

	QWinMainApp win;
	QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &win, SLOT(handleAppMessage(const QString&)));

	return app.exec();
}
