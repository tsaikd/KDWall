#ifndef _KDWALL_WINMAINAPP_H
#define _KDWALL_WINMAINAPP_H

#include "stable.h"

class QWallMgr;
class QPicFinder;

class QWinMainApp : public QWidget
{
	Q_OBJECT
private:
	void _init();
public:
	QWinMainApp() { _init(); }

protected:
	virtual void closeEvent(QCloseEvent* event);

protected slots:
	void choiceDirAddToTree();
	void removeCurDirFromTree();
	void updateTreePicCount(const QString& sDir);

protected:
	QWallMgr* m_wallmgr;
	QPicFinder* m_picfinder;

	QComboBox* m_cboProfile;
	QTreeWidget* m_treePicDir;
};

#endif//_KDWALL_WINMAINAPP_H
