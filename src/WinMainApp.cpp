#include "WinMainApp.h"

#include "WallMgr.h"
#include "PicFinder.h"
#include "TreePicDir.h"

enum TREE_PICDIR_COLUMN {
	TREE_PICDIR_COLUMN_PATH,
	TREE_PICDIR_COLUMN_PICCOUNT,
};

void QWinMainApp::_init()
{
	m_wallmgr = new QWallMgr(this);
	QPicFinder*& picfinder = m_picfinder;
	picfinder = new QPicFinder(this);

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
				connect(btn, SIGNAL(clicked()), this, SLOT(choiceDirAddToTree()));
				lot3->addWidget(btn);
			}
			{
				QPushButton* btn = new QPushButton(this);
				btn->setText(tr("-"));
				btn->setFixedWidth(20);
				connect(btn, SIGNAL(clicked()), this, SLOT(removeCurDirFromTree()));
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

	connect(picfinder, SIGNAL(dirFoundOver(const QString&)), this, SLOT(updateTreePicCount(const QString&)));
	picfinder->start(QThread::LowestPriority);

	// test code
	QTreeWidgetItem* item = new QTreeWidgetItem();
	item->setText(TREE_PICDIR_COLUMN_PATH, "C:/beauty");
	item->setText(TREE_PICDIR_COLUMN_PICCOUNT, "0");
	m_treePicDir->addTopLevelItem(item);
	m_picfinder->addPicDir("C:/beauty");

	show();
}

void QWinMainApp::closeEvent(QCloseEvent* event)
{
	QWallMgr& wallmgr = *m_wallmgr;

	QWallPaperParam wall;
	wallmgr.getWallPaper(wall);
	if (wall != wallmgr.initWall())
		wallmgr.setWallPaper(wallmgr.initWall());

	QPicFinder*& picfinder = m_picfinder;
	picfinder->quit();
	if (!picfinder->wait(10000)) {
		QTRACE() << "PicFinder wait timeout";
		picfinder->exit(1);
	}

	QWidget::closeEvent(event);
}

void QWinMainApp::addDirToTree(const QString& sDir)
{
	QTreePicDir*& tree = m_treePicDir;
	QTreeWidgetItem* item = new QTreeWidgetItem();
	item->setText(TREE_PICDIR_COLUMN_PATH, sDir);
	item->setText(TREE_PICDIR_COLUMN_PICCOUNT, "0");
	tree->addTopLevelItem(item);

	QPicFinder*& picfinder = m_picfinder;
	picfinder->addPicDir(sDir);
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
	QTreePicDir*& tree = m_treePicDir;
	QList<QTreeWidgetItem*> list = tree->selectedItems();
	QTreeWidgetItem* item;
	int i;

	while (!list.isEmpty()) {
		item = list.takeFirst();
		i = tree->indexOfTopLevelItem(item);
		tree->takeTopLevelItem(i);
	}
}

void QWinMainApp::updateTreePicCount(const QString& sDir)
{
	QTreePicDir*& tree = m_treePicDir;
	QList<QTreeWidgetItem*> list = tree->findItems(sDir, Qt::MatchExactly, TREE_PICDIR_COLUMN_PATH);
	QTreeWidgetItem* item;
	QString count;

	if (!list.isEmpty()) {
		QSqlQuery sql;
		sql.prepare("SELECT count(*) FROM piclist WHERE dir = ?;");
		sql.addBindValue(sDir);
		sql.exec();
		if (sql.next()) {
			count = sql.value(0).toString();
			while (!list.isEmpty()) {
				item = list.takeFirst();
				item->setText(TREE_PICDIR_COLUMN_PICCOUNT, count);
			}
		}
	}
}
