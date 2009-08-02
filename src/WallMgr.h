#ifndef _KDWALL_WALLMGR_H
#define _KDWALL_WALLMGR_H

#include "stable.h"

class QConfMainApp;
class QWallPaperParam;

typedef QList<QWallPaperParam*> QWallParamList;

class QWallPaperParam
{
private:
	void _init();
public:
	QWallPaperParam() { _init(); }
	~QWallPaperParam() { close(); }
	QWallPaperParam& close();

	// set by sender
	QString m_path;
	enum STYLE {
		STYLE_EXPAND,
		STYLE_CENTER,
		STYLE_TILE,
	} m_style;

	// set by QWallMgr
	QString m_tmpPath;
	bool m_useOrigin; // sender can set

	bool operator == (const QWallPaperParam& x) const;
	bool operator != (const QWallPaperParam& x) const { return !(*this == x); }
};

class QWallCacheMaker : public QThread
{
private:
	void _init();
public:
	QWallCacheMaker(QConfMainApp& conf, QWallParamList& wallList, QMutex* wallMutex, QObject* parent = NULL)
		:	QThread(parent), m_conf(&conf), m_wallList(&wallList), m_wallMutex(wallMutex)
	{ _init(); }
	~QWallCacheMaker();

	QWallCacheMaker& addWall(QWallPaperParam* wall);
	bool prepareCacheImg(QWallPaperParam& wall);
	int wallListCount();

protected:
	virtual void run();

protected:
	QConfMainApp* m_conf;
	QWallParamList* m_wallList;
	QMutex* m_wallMutex;
	QWallParamList m_beMadeWallList;
	QMutex m_beMadeWallMutex;
};

class QWallMgr : public QObject
{
	Q_OBJECT
private:
	void _init();
public:
	QWallMgr(QConfMainApp* conf)
		:	QObject((QObject*)conf), m_conf(conf), m_mutex(QMutex::Recursive)
	{ _init(); }
	~QWallMgr();

protected:
	bool getWallPaper(QWallPaperParam& wall);
	bool setWallPaper(QWallPaperParam& wall);
public slots:
	bool setRandWallPaper();
	void clearCacheWall();

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
	QWallCacheMaker* m_cacheMaker;
	QWallParamList m_wallList;
};

#endif//_KDWALL_WALLMGR_H
