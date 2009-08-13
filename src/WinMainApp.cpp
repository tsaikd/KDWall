#include "WinMainApp.h"

#include "ConfMainApp.h"
#include "WinConfMainApp.h"
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
	DEWRP(QConfMainApp, conf, m_conf, new QConfMainApp(this));
	conf.m_mainWidget = this;
	DECOP(QWallMgr, conf, wallmgr);
	DECCV(bool, bFirstChangeWallPaper);
	bFirstChangeWallPaper = false;

	QAction* act = new QAction(this);
	act->setShortcut(Qt::Key_Escape);
	addAction(act);
	connect(act, SIGNAL(triggered()), this, SLOT(close()));

	// Tray Icon
	DEWRP(QSystemTrayIcon, tray, m_tray, new QSystemTrayIcon(this));
	tray.setIcon(QIcon(":/icon/MagicKD.ico"));
	connect(&tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
	{	// Tray menu
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
		QLabel*& lbl = m_lblCurWallPath;
		lbl = new QLabel(this);
		lot->addWidget(lbl);
	}
	{
		QHBoxLayout* lot2 = new QHBoxLayout();
		{
			DEWRP(QTreePicDir, tree, m_treePicDir, new QTreePicDir(this));
			tree.setRootIsDecorated(false);
			QStringList headers;
			headers << tr("Path");
			headers << tr("Count");
			tree.setHeaderLabels(headers);
			tree.setSortingEnabled(true);
			connect(&tree, SIGNAL(dropUrls(QList<QUrl>&)), this, SLOT(addUrlsToTree(QList<QUrl>&)));
			connect(&tree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(treeItemChanged(QTreeWidgetItem*, int)));
			lot2->addWidget(&tree);
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
			btn->setText(tr("&Config"));
			connect(btn, SIGNAL(clicked()), this, SLOT(showWinConf()));
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

	connect(&wallmgr, SIGNAL(changingWallPaper(const QWallPaperParam&)), this, SLOT(changingWallPaper(const QWallPaperParam&)));
	connect(&wallmgr, SIGNAL(changedWallPaper(const QWallPaperParam&)), this, SLOT(changedWallPaper(const QWallPaperParam&)));

	if (wallmgr.setRandWallPaper())
		bFirstChangeWallPaper = true;

	DECOP(QPicFinder, conf, picfinder);
	connect(&picfinder, SIGNAL(dirFoundSome(const QString&)), this, SLOT(updateTreePicCount(const QString&)));
	connect(&picfinder, SIGNAL(dirFoundOver(const QString&)), this, SLOT(updateTreePicCount(const QString&)));
	picfinder.start(QThread::LowestPriority);

	// post init
	{
		DECOP(QDBMgr, conf, db);
		QStringList dirs = db.getDirList();
		for (int i=0 ; i<dirs.count() ; i++) {
			addDirToTree(dirs[i]);
		}
	}

	DECOV(bool, conf, initWithoutWindow);
	if (initWithoutWindow) {
		tray.show();
	} else {
		show();
	}
}

void QWinMainApp::closeEvent(QCloseEvent* e)
{
	DECCP(QConfMainApp, conf);
	DECOP(QPicFinder, conf, picfinder);

	conf.m_closing = true;
	conf.m_closingPicfinder = true;

	picfinder.quit();
	if (!picfinder.wait(10000)) {
		qWarning() << "Q" << "Z";
		QTRACE() << "PicFinder wait timeout";
		picfinder.exit(1);
	}

	QWidget::closeEvent(e);
}

void QWinMainApp::changeEvent(QEvent* e)
{
	if (e->type() == QEvent::WindowStateChange) {
		if (e->spontaneous()) {
			QWindowStateChangeEvent* e1 = (QWindowStateChangeEvent*)e;
			QTRACE() << e1->oldState();
			DECCP(QSystemTrayIcon, tray);
			if (isMinimized()) {
				m_rect = geometry();
#if defined(Q_WS_WIN)
				setWindowFlags((windowFlags() | Qt::FramelessWindowHint) & (~Qt::Window));
#endif//defined(Q_WS_WIN)
				hide();
				tray.show();
			}
		}
	}
}

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
	DECRP(QTreePicDir, tree, m_treePicDir);

	if (!db.isDirExists(sDir)) {
		DECOP(QPicFinder, conf, picfinder);
		picfinder.addPicDir(sDir);
	}

	QList<QTreeWidgetItem*> list = tree.findItems(sDir, Qt::MatchExactly, TREE_PICDIR_COLUMN_PATH);
	if (list.isEmpty()) {
		QTreeWidgetItem* item = new QTreeWidgetItem();
		item->setText(TREE_PICDIR_COLUMN_PATH, sDir);
		item->setText(TREE_PICDIR_COLUMN_PICCOUNT, QString::number(db.getDirPicCount(sDir)));
		item->setCheckState(TREE_PICDIR_COLUMN_PATH, db.dirEnable(sDir) ? Qt::Checked : Qt::Unchecked);
		tree.addTopLevelItem(item);
		tree.resizeColumnToContents(TREE_PICDIR_COLUMN_PATH);
		tree.resizeColumnToContents(TREE_PICDIR_COLUMN_PICCOUNT);
		qApp->processEvents();
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
	DECRP(QTreePicDir, tree, m_treePicDir);
	QList<QTreeWidgetItem*> list = tree.selectedItems();
	QTreeWidgetItem* item;
	int i;

	while (!list.isEmpty()) {
		item = list.takeFirst();
		const QString& sDir = item->text(TREE_PICDIR_COLUMN_PATH);
		picfinder.removePicDir(sDir);
		i = tree.indexOfTopLevelItem(item);
		tree.takeTopLevelItem(i);
	}
}

void QWinMainApp::refreshCurDirFromTree()
{
	DECCP(QConfMainApp, conf);

	DECOP(QPicFinder, conf, picfinder);
	DECRP(QTreePicDir, tree, m_treePicDir);
	QTreeWidgetItem* item;

	if (qApp->keyboardModifiers() == Qt::AltModifier) {
		for (int i=0 ; i<tree.topLevelItemCount() ; i++) {
			item = tree.topLevelItem(i);

			const QString& sDir = item->text(TREE_PICDIR_COLUMN_PATH);
			picfinder.removePicDir(sDir);
			updateTreePicCount(sDir);
			addDirToTree(sDir);
		}
	} else {
		QList<QTreeWidgetItem*> list = tree.selectedItems();
		while (!list.isEmpty()) {
			item = list.takeFirst();

			const QString& sDir = item->text(TREE_PICDIR_COLUMN_PATH);
			picfinder.removePicDir(sDir);
			updateTreePicCount(sDir);
			addDirToTree(sDir);
		}
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
	DECRP(QTreePicDir, tree, m_treePicDir);
	QList<QTreeWidgetItem*> list = tree.findItems(sDir, Qt::MatchExactly, TREE_PICDIR_COLUMN_PATH);
	QTreeWidgetItem* item;

	if (!list.isEmpty()) {
		QString count = QString::number(db.getDirPicCount(sDir));
		while (!list.isEmpty()) {
			item = list.takeFirst();
			item->setText(TREE_PICDIR_COLUMN_PICCOUNT, count);
		}
		tree.resizeColumnToContents(TREE_PICDIR_COLUMN_PICCOUNT);
		qApp->processEvents();
	}
}

void QWinMainApp::changingWallPaper(const QWallPaperParam& wall)
{
	QString path = QDir::toNativeSeparators(wall.m_path);
	DECCP(QSystemTrayIcon, tray);
	tray.setIcon(QIcon(":/icon/icon_changingwall.ico"));
	tray.setToolTip(tr("Changing: %1").arg(path));
}

void QWinMainApp::changedWallPaper(const QWallPaperParam& wall)
{
	QString path = QDir::toNativeSeparators(wall.m_path);
	DECCP(QSystemTrayIcon, tray);
	tray.setIcon(QIcon(":/icon/MagicKD.ico"));
	tray.setToolTip(path);

	DECCP(QLabel, lblCurWallPath);
	lblCurWallPath.setText(QDir::toNativeSeparators(path));
}

void QWinMainApp::treeItemChanged(QTreeWidgetItem* item, int column)
{
	if (column == TREE_PICDIR_COLUMN_PATH) {
		DECCP(QConfMainApp, conf);
		DECOP(QDBMgr, conf, db);
		DECOP(QWallMgr, conf, wallmgr);

		const QString& sDir = item->text(TREE_PICDIR_COLUMN_PATH);
		if (Qt::Checked == item->checkState(column)) {
			db.setDirEnable(sDir, true);
		} else {
			db.setDirEnable(sDir, false);
		}
		wallmgr.clearCacheWall();
	}
}

void QWinMainApp::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
	DECCP(QSystemTrayIcon, tray);
	switch (reason) {
	case QSystemTrayIcon::Trigger:
#if defined(Q_WS_WIN)
		setWindowFlags((windowFlags() | Qt::Window) & (~Qt::FramelessWindowHint));
#endif//defined(Q_WS_WIN)
		showNormal();
		activateWindow();
		tray.hide();
		if (m_rect.isValid())
			setGeometry(m_rect);
		break;
	default:
		break;
	}
}

void QWinMainApp::showWinConf()
{
	QWinConfMainApp* winConf = new QWinConfMainApp(*m_conf, this);
	winConf->show();
}
