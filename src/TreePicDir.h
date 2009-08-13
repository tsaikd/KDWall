#ifndef _KDWALL_TREEPICDIR_H
#define _KDWALL_TREEPICDIR_H

#include "stable.h"

class QTreePicDir : public QTreeWidget
{
	Q_OBJECT
private:
	void _init();
public:
	QTreePicDir(QWidget* parent = NULL)
		:	QTreeWidget(parent)
	{ _init(); }

signals:
	void dropUrls(QList<QUrl>& urls);

protected:
	virtual void dragEnterEvent(QDragEnterEvent* e);
	virtual void dropEvent(QDropEvent* e);
};

#endif//_KDWALL_TREEPICDIR_H
