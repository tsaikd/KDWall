#ifndef _KDWALL_WINMAINAPP_H
#define _KDWALL_WINMAINAPP_H

#include "stable.h"

enum APPMSG {
	APPMSG_SHOWGUI,
};

class QConfMainApp;
class QTreePicDir;
class QWallPaperParam;

class QWinMainApp : public QWidget
{
	Q_OBJECT
private:
	void _init();
public:
	QWinMainApp() { _init(); }

protected:
	virtual void closeEvent(QCloseEvent* event);
	virtual bool winEvent(MSG* message, long* result);

public slots:
	void handleAppMessage(const QString& sMsg);

protected:
	void addDirToTree(const QString& sDir);
protected slots:
	void addUrlsToTree(QList<QUrl>& urls);
	void choiceDirAddToTree();
	void removeCurDirFromTree();
	void refreshCurDirFromTree();
	void updateTreePicCount(const QString& sDir);
	void trayToChangingWall(const QWallPaperParam& wall);
	void trayToChangedWall(const QWallPaperParam& wall);

	// for trayicon
	void trayActivated(QSystemTrayIcon::ActivationReason reason);

protected:
	QConfMainApp* m_conf;
	bool m_bFirstChangeWallPaper;

	QSystemTrayIcon* m_tray;
	QComboBox* m_cboProfile;
	QTreePicDir* m_treePicDir;
};

#endif//_KDWALL_WINMAINAPP_H
