#include "mainApp.h"

#include "QtSingleApplication"
#include "WinMainApp.h"

enum APPMSG {
	APPMSG_SHOWGUI,
};

int main(int argc, char* argv[])
{
    QtSingleApplication app("KDWallApp", argc, argv);
	if (app.sendMessage(QString::number(APPMSG_SHOWGUI)))
		return 0;

	QWinMainApp win;

	return app.exec();
}
