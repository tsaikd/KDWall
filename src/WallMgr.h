#ifndef _KDWALL_WALLMGR_H
#define _KDWALL_WALLMGR_H

#include "stable.h"

class QWallPaperParam
{
private:
	void _init();
public:
	QWallPaperParam() {}
	QWallPaperParam& close();

	QString m_path;
	enum STYLE {
		STYLE_EXPAND,
		STYLE_CENTER,
		STYLE_TILE,
	} m_style;

	bool operator == (const QWallPaperParam& x) const;
	bool operator != (const QWallPaperParam& x) const { return !(*this == x); }
};

class QWallMgr : public QObject
{
private:
	void _init();
public:
	QWallMgr(QObject* parent = NULL)
		:	QObject(parent)
	{ _init(); }
	QWallMgr& close();

	QWallPaperParam& initWall() { return m_initWall; }
	bool getWallPaper(QWallPaperParam& wall);
	bool setWallPaper(const QWallPaperParam& wall);

protected:
	QWallPaperParam m_initWall;
	QList<QWallPaperParam> m_listWall;
};

#endif//_KDWALL_WALLMGR_H
