#include "WinMainApp.h"

#include "ConfMainApp.h"
#include "DBMgr.h"
#include "WallMgr.h"
#include "PicFinder.h"
#include "TreePicDir.h"

enum TREE_PICDIR_COLUMN {
	TREE_PICDIR_COLUMN_PATH,
	TREE_PICDIR_COLUMN_PICCOUNT,
};

void QWinMainApp::_init()
{
	m_conf = new QConfMainApp(this);
	DECCP(QConfMainApp, conf);
	DECOP(QWallMgr, conf, wallmgr);
	DECCV(bool, bFirstChangeWallPaper);
	bFirstChangeWallPaper = false;

	QAction* act = new QAction(this);
	act->setShortcut(Qt::Key_Escape);
	addAction(act);
	connect(act, SIGNAL(triggered()), this, SLOT(close()));

	{ // Tray Icon
		m_tray = new QSystemTrayIcon(this);
		DECCP(QSystemTrayIcon, tray);
		tray.setIcon(QIcon(":/icon/MagicKD.ico"));
		connect(&tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
		QMenu* menu = new QMenu(this);
		menu->addAction(tr("&Random Picture"), &wallmgr, SLOT(setRandWallPaper()));
		menu->addAction(tr("&Quit"), this, SLOT(close()));
		tray.setContextMenu(menu);
	}

	QVBoxLayout* lot = new QVBoxLayout();
	{
		QHBoxLayout* lot2 = new QHBoxLayout();
		{
			QLabel* lbl = new QLabel(this);
			lbl->setText(tr("Profile:"));
			lot2->addWidget(lbl);
		}
		{
			QComboBox*& cbo = m_cboProfile;
			cbo = new QComboBox(this);
			lot2->addWidget(cbo);
		}
		lot->addLayout(lot2);
	}
	{
		QHBoxLayout* lot2 = new QHBoxLayout();
		{
			QTreePicDir*& tree = m_treePicDir;
			tree = new QTreePicDir(this);
			tree->setRootIsDecorated(false);
			QStringList headers;
			headers << tr("Path");
			headers << tr("Count");
			tree->setHeaderLabels(headers);
			connect(tree, SIGNAL(dropUrls(QList<QUrl>&)), this, SLOT(addUrlsToTree(QList<QUrl>&)));
			lot2->addWidget(tree);
		}
		{
			QVBoxLayout* lot3 = new QVBoxLayout();
			{
				QPushButton* btn = new QPushButton(this);
				btn->setText(tr("+"));
				btn->setFixedWidth(20);
				btn->setToolTip(tr("Add other picture dir"));
				connect(btn, SIGNAL(clicked()), this, SLOT(choiceDirAddToTree()));
				lot3->addWidget(btn);
			}
			{
				QPushButton* btn = new QPushButton(this);
				btn->setText(tr("-"));
				btn->setFixedWidth(20);
				btn->setToolTip(tr("Remove selected picture dir"));
				connect(btn, SIGNAL(clicked()), this, SLOT(removeCurDirFromTree()));
				lot3->addWidget(btn);
			}
			{
				QPushButton* btn = new QPushButton(this);
				btn->setText(tr("R"));
				btn->setFixedWidth(20);
				btn->setToolTip(tr("Search selected picture dir again"));
				connect(btn, SIGNAL(clicked()), this, SLOT(refreshCurDirFromTree()));
				lot3->addWidget(btn);
			}
			lot2->addLayout(lot3);
		}
		lot->addLayout(lot2);
	}
	{
		QHBoxLayout* lot2 = new QHBoxLayout();
		{
			QPushButton* btn = new QPushButton(this);
			btn->setText(tr("&RandPic"));
			btn->setToolTip(tr("Select a random picture from database and setup wallpaper"));
			connect(btn, SIGNAL(clicked()), &wallmgr, SLOT(setRandWallPaper()));
			lot2->addWidget(btn);
		}
		{
			QPushButton* btn = new QPushButton(this);
			btn->setText(tr("&Quit"));
			connect(btn, SIGNAL(clicked()), this, SLOT(close()));
			lot2->addWidget(btn);
		}
		lot->addLayout(lot2);
	}
	setLayout(lot);

	if (wallmgr.setRandWallPaper())
		bFirstChangeWallPaper = true;

	DECOP(QPicFinder, conf, picfinder);
	connect(&picfinder, SIGNAL(dirFoundSome(const QString&)), this, SLOT(updateTreePicCount(const QString&)));
	connect(&picfinder, SIGNAL(dirFoundOver(const QString&)), this, SLOT(updateTreePicCount(const QString&)));
	picfinder.start(QThread::LowestPriority);

	connect(&wallmgr, SIGNAL(changingWallPaper(const QWallPaperParam&)), this, SLOT(trayToChangingWall(const QWallPaperParam&)));
	connect(&wallmgr, SIGNAL(changedWallPaper(const QWallPaperParam&)), this, SLOT(trayToChangedWall(const QWallPaperParam&)));

	// post init
	{
		DECOP(QDBMgr, conf, db);
		QStringList dirs = db.getDirList();
		for (int i=0 ; i<dirs.count() ; i++) {
			addDirToTree(dirs[i]);
		}
	}

	show();
}

void QWinMainApp::closeEvent(QCloseEvent* event)
{
	DECCP(QConfMainApp, conf);

	DECOP(QWallMgr, conf, wallmgr);
	DECOP(QPicFinder, conf, picfinder);

	conf.m_closing = true;
	conf.m_closingPicfinder = true;

	picfinder.quit();
	if (!picfinder.wait(10000)) {
		qWarning() << "Q" << "Z";
		QTRACE() << "PicFinder wait timeout";
		picfinder.exit(1);
	}

	QWallPaperParam wall;
	wallmgr.getWallPaper(wall);
	if (wall != wallmgr.initWall())
		wallmgr.setWallPaper(wallmgr.initWall());

	QWidget::closeEvent(event);
}

#ifdef Q_WS_WIN
#include <Windows.h>
bool QWinMainApp::winEvent(MSG* message, long* result)
{
	switch (message->message) {
	case WM_SYSCOMMAND:
		if (message->wParam == SC_MINIMIZE) {
			DECCP(QSystemTrayIcon, tray);
			tray.show();
			hide();
			return true;
		}
		break;
	}
	return QWidget::winEvent(message, result);
}
#endif//Q_WS_WIN

void QWinMainApp::handleAppMessage(const QString& sMsg)
{
	APPMSG msg = (APPMSG)sMsg.toInt();
	switch (msg) {
	case APPMSG_SHOWGUI:
		raise();
		activateWindow();
		break;
	default:
		QTRACE() << "Unknown APPMSG:" << sMsg;
	}
}

void QWinMainApp::addDirToTree(const QString& sDir)
{
	DECCP(QConfMainApp, conf);
	DECOP(QDBMgr, conf, db);

	QTreePicDir*& tree = m_treePicDir;
	QList<QTreeWidgetItem*> list = tree->findItems(sDir, Qt::MatchExactly, TREE_PICDIR_COLUMN_PATH);
	if (list.isEmpty()) {
		QTreePicDir*& tree = m_treePicDir;
		QTreeWidgetItem* item = new QTreeWidgetItem();
		item->setText(TREE_PICDIR_COLUMN_PATH, sDir);
		item->setText(TREE_PICDIR_COLUMN_PICCOUNT, QString::number(db.getDirPicCount(sDir)));
		tree->addTopLevelItem(item);
		qApp->processEvents();
	}

	if (!db.isDirExists(sDir)) {
		DECOP(QPicFinder, conf, picfinder);
		picfinder.addPicDir(sDir);
	}
}

void QWinMainApp::addUrlsToTree(QList<QUrl>& urls)
{
	for (int i=0 ; i<urls.count() ; i++) {
		QFileInfo fi(urls[i].toLocalFile());
		if (fi.isDir()) {
			addDirToTree(fi.absoluteFilePath());
		}
	}
}

void QWinMainApp::choiceDirAddToTree()
{
	const QString& sDir = QFileDialog::getExistingDirectory(this, tr("Add picture directory"));
	if (sDir.isEmpty() || !QFile::exists(sDir))
		return;

	addDirToTree(sDir);
}

void QWinMainApp::removeCurDirFromTree()
{
	DECCP(QConfMainApp, conf);

	DECOP(QPicFinder, conf, picfinder);
	QTreePicDir*& tree = m_treePicDir;
	QList<QTreeWidgetItem*> list = tree->selectedItems();
	QTreeWidgetItem* item;
	int i;

	while (!list.isEmpty()) {
		item = list.takeFirst();
		const QString& sDir = item->text(TREE_PICDIR_COLUMN_PATH);
		picfinder.removePicDir(sDir);
		i = tree->indexOfTopLevelItem(item);
		tree->takeTopLevelItem(i);
	}
}

void QWinMainApp::refreshCurDirFromTree()
{
	DECCP(QConfMainApp, conf);

	DECOP(QPicFinder, conf, picfinder);
	QTreePicDir*& tree = m_treePicDir;
	QList<QTreeWidgetItem*> list = tree->selectedItems();
	QTreeWidgetItem* item;

	while (!list.isEmpty()) {
		item = list.takeFirst();
		const QString& sDir = item->text(TREE_PICDIR_COLUMN_PATH);
		picfinder.removePicDir(sDir);
		updateTreePicCount(sDir);
		addDirToTree(sDir);
	}
}

void QWinMainApp::updateTreePicCount(const QString& sDir)
{
	DECCP(QConfMainApp, conf);
	DECOP(QWallMgr, conf, wallmgr);
	DECCV(bool, bFirstChangeWallPaper);

	if (!bFirstChangeWallPaper) {
		if (wallmgr.setRandWallPaper()) {
			bFirstChangeWallPaper = true;
		}
	}

	DECOP(QDBMgr, conf, db);
	QTreePicDir*& tree = m_treePicDir;
	QList<QTreeWidgetItem*> list = tree->findItems(sDir, Qt::MatchExactly, TREE_PICDIR_COLUMN_PATH);
	QTreeWidgetItem* item;

	if (!list.isEmpty()) {
		QString count = QString::number(db.getDirPicCount(sDir));
		while (!list.isEmpty()) {
			item = list.takeFirst();
			item->setText(TREE_PICDIR_COLUMN_PICCOUNT, count);
		}
		qApp->processEvents();
	}
}

void QWinMainApp::trayToChangingWall(const QWallPaperParam& wall)
{
	DECCP(QSystemTrayIcon, tray);
	tray.setIcon(QIcon(":/icon/icon_changingwall.ico"));
	tray.setToolTip(tr("Changing: %1").arg(wall.m_path));
}

void QWinMainApp::trayToChangedWall(const QWallPaperParam& wall)
{
	DECCP(QSystemTrayIcon, tray);
	tray.setIcon(QIcon(":/icon/MagicKD.ico"));
	tray.setToolTip(wall.m_path);
}

void QWinMainApp::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
	DECCP(QSystemTrayIcon, tray);
	switch (reason) {
	case QSystemTrayIcon::Trigger:
		show();
		activateWindow();
		tray.hide();
		break;
	}
}
