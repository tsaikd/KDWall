#ifndef _KDWALL_WINMAINAPP_H
#define _KDWALL_WINMAINAPP_H

#include "stable.h"

enum APPMSG {
	APPMSG_SHOWGUI,
};

class QConfMainApp;
class QWinConfMainApp;
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
	virtual void closeEvent(QCloseEvent* e);
	virtual void changeEvent(QEvent* e);

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
	void changingWallPaper(const QWallPaperParam& wall);
	void changedWallPaper(const QWallPaperParam& wall);
	void treeItemChanged(QTreeWidgetItem* item, int column);
	void showWinConf();

	// for trayicon
	void trayActivated(QSystemTrayIcon::ActivationReason reason);

protected:
	QConfMainApp* m_conf;
	bool m_bFirstChangeWallPaper;

	QSystemTrayIcon* m_tray;
	QComboBox* m_cboProfile;
	QTreePicDir* m_treePicDir;
	QLabel* m_lblCurWallPath;
};

#endif//_KDWALL_WINMAINAPP_H
