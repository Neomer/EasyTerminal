#include "SettingsDlg.h"
#include "ui_SettingsDlg.h"

#include <QDir>
#include <QLocale>

SettingsDlg::SettingsDlg(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SettingsDlg)
{
	ui->setupUi(this);
	
	connect(ui->cmdOK, SIGNAL(clicked(bool)), this, SLOT(accept()));
	connect(ui->cmdCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));

	QDir dir(qApp->applicationDirPath().append("/intl/"));
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	QFileInfoList list = dir.entryInfoList();
	
	for (int i = 0; i < list.size(); ++i) 
	{
        QFileInfo fileInfo = list.at(i);
		QLocale l(fileInfo.baseName());
		ui->languageComboBox->addItem(
					fileInfo.baseName().append(" - ").append(QLocale::languageToString(l.language())),
					fileInfo.baseName());
    }
}

SettingsDlg::~SettingsDlg()
{
	delete ui;
}



void SettingsDlg::getSettings(Settings &result)
{
	result.setLocaleName(ui->languageComboBox->currentData().toString());
}

void SettingsDlg::setSettings(Settings &value)
{
	for (int i = 0; i < ui->languageComboBox->count(); i++)
		if (ui->languageComboBox->itemData(i).toString() == value.getLocaleName()) 
		{
			ui->languageComboBox->setCurrentIndex(i);
			break;
		}
}
