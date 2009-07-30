#ifndef _KDWALL_WINMAINAPP_H
#define _KDWALL_WINMAINAPP_H

#include "stable.h"

enum APPMSG {
	APPMSG_SHOWGUI,
};

class QConfMainApp;
class QTreePicDir;

class QWinMainApp : public QWidget
{
	Q_OBJECT
private:
	void _init();
public:
	QWinMainApp() { _init(); }

protected:
	virtual void closeEvent(QCloseEvent* event);

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

protected:
	QConfMainApp* m_conf;
	bool m_bFirstChangeWallPaper;

	QComboBox* m_cboProfile;
	QTreePicDir* m_treePicDir;
};

#endif//_KDWALL_WINMAINAPP_H
