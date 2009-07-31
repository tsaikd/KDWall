#ifndef _KDWALL_WALLMGR_H
#define _KDWALL_WALLMGR_H

#include "stable.h"

class QConfMainApp;

class QWallPaperParam
{
private:
	void _init();
public:
	QWallPaperParam() { _init(); }
	QWallPaperParam& close();

	QString m_path;
	enum STYLE {
		STYLE_EXPAND,
		STYLE_CENTER,
		STYLE_TILE,
	} m_style;
	bool m_useOrigin;

	bool operator == (const QWallPaperParam& x) const;
	bool operator != (const QWallPaperParam& x) const { return !(*this == x); }
};

class QWallMgr : public QObject
{
	Q_OBJECT
private:
	void _init();
public:
	QWallMgr(QConfMainApp* conf)
		:	m_conf(conf), QObject((QObject*)conf), m_mutex(QMutex::Recursive)
	{ _init(); }

	QWallPaperParam& initWall() { QMutexLocker locker(&m_mutex); return m_initWall; }

public slots:
	bool getWallPaper(QWallPaperParam& wall);
	bool setWallPaper(QWallPaperParam& wall);
	bool setRandWallPaper();

signals:
	void changingWallPaper(const QWallPaperParam& wall);
	void changedWallPaper(const QWallPaperParam& wall);

protected:
	virtual void timerEvent(QTimerEvent* e);

protected:
	QConfMainApp* m_conf;
	QWallPaperParam m_initWall;
	QMutex m_mutex;
	int m_timerId;
};

#endif//_KDWALL_WALLMGR_H
