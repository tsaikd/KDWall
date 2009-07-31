#include "TreePicDir.h"

void QTreePicDir::_init()
{
	setAcceptDrops(true);
	setDragDropMode(QAbstractItemView::InternalMove);
}

void QTreePicDir::dragEnterEvent(QDragEnterEvent* e)
{
	if (e->mimeData()->hasFormat("text/uri-list")) {
		e->acceptProposedAction();
	}
}

void QTreePicDir::dropEvent(QDropEvent* e)
{
	QList<QUrl> urls = e->mimeData()->urls();
	if (urls.isEmpty())
		return;

	emit(dropUrls(urls));
}
