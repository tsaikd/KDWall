#ifndef _KDWALL_QWINCONFMAINAPP_H
#define _KDWALL_QWINCONFMAINAPP_H

#include "stable.h"

class QConfMainApp;

class QWinConfMainApp : public QWidget
{
	Q_OBJECT
private:
	void _init();
public:
	QWinConfMainApp(QConfMainApp& conf, QWidget* parent = NULL)
		:	QWidget(parent), m_conf(&conf)
	{ _init(); }

protected:
	void newWinConfBool(bool* var, QCheckBox*& chk, const QString& text, QLayout* lot);
	void newWinConfInt(int* var, QLineEdit*& edt, const QString& text, QVBoxLayout* lot);

protected slots:
	void saveConf();

protected:
	QConfMainApp* m_conf;

	QCheckBox* m_initWithoutWindow;

	QLineEdit* m_pichist_max_num;

	QLineEdit* m_wall_timer_sec;
	QLineEdit* m_max_cache_image;
	QCheckBox* m_disable_cache_warning;
	QCheckBox* m_still_run_when_fullscreen;

	QCheckBox* m_ignoreImageFormatSupportWarning;
	QLineEdit* m_picfinder_refresh_msec;
};

#endif//_KDWALL_QWINCONFMAINAPP_H
