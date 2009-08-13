#include "WinConfMainApp.h"

#include "ConfMainApp.h"

void QWinConfMainApp::_init()
{
	DECCP(QConfMainApp, conf);
//	int labelWidth = 100;

	setWindowFlags(windowFlags() | Qt::Window);

	QVBoxLayout* lot = new QVBoxLayout();
	{
		newWinConfBool(&conf.m_initWithoutWindow, m_initWithoutWindow, tr("Start without window"), lot);

		newWinConfInt(&conf.m_pichist_max_num, m_pichist_max_num, tr("Picture history max number"), lot);

		newWinConfInt(&conf.m_wall_timer_sec, m_wall_timer_sec, tr("Wallpaper change timer (second)"), lot);
		newWinConfInt(&conf.m_max_cache_image, m_max_cache_image, tr("Cache image max number"), lot);
		newWinConfBool(&conf.m_disable_cache_warning, m_disable_cache_warning, tr("Disable cache warning"), lot);

		newWinConfBool(&conf.m_ignoreImageFormatSupportWarning, m_ignoreImageFormatSupportWarning, tr("Ignore image format support warning"), lot);
		newWinConfInt(&conf.m_picfinder_refresh_msec, m_picfinder_refresh_msec, tr("Picture finder refresh timer (msec)"), lot);
	}
	{
		QHBoxLayout* lot2 = new QHBoxLayout();
		{
			QPushButton* btn = new QPushButton(this);
			btn->setText(tr("&Ok"));
			connect(btn, SIGNAL(clicked()), this, SLOT(saveConf()));
			lot2->addWidget(btn);
		}
		{
			QPushButton* btn = new QPushButton(this);
			btn->setText(tr("&Cancel"));
			connect(btn, SIGNAL(clicked()), this, SLOT(close()));
			lot2->addWidget(btn);
		}
		lot->addLayout(lot2);
	}
	setLayout(lot);
}

void QWinConfMainApp::newWinConfBool(bool* var, QCheckBox*& chk, const QString& text, QLayout* lot)
{
	chk = new QCheckBox(this);
	chk->setText(text);
	chk->setChecked(*var ? Qt::Checked : Qt::Unchecked);
	lot->addWidget(chk);
}

void QWinConfMainApp::newWinConfInt(int* var, QLineEdit*& edt, const QString& text, QVBoxLayout* lot)
{
	QHBoxLayout* lot2 = new QHBoxLayout();
	{
		QLabel* lbl = new QLabel(this);
		lbl->setText(text);
		lot2->addWidget(lbl);
	}
	{
		edt = new QLineEdit(this);
		edt->setText(QString::number(*var));
		lot2->addWidget(edt);
	}
	lot->addLayout(lot2);
}

void QWinConfMainApp::saveConf()
{
	DECCP(QConfMainApp, conf);

	conf.m_initWithoutWindow = (m_initWithoutWindow->checkState() == Qt::Checked) ? true : false;

	conf.m_pichist_max_num = m_pichist_max_num->text().toInt();

	conf.m_wall_timer_sec = m_wall_timer_sec->text().toInt();
	conf.m_max_cache_image = m_max_cache_image->text().toInt();

	conf.m_ignoreImageFormatSupportWarning = (m_ignoreImageFormatSupportWarning->checkState() == Qt::Checked) ? true : false;
	conf.m_picfinder_refresh_msec = m_picfinder_refresh_msec->text().toInt();

	conf.save();
	close();
}
