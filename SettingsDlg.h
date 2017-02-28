#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <QDialog>
#include <model/Settings.h>

namespace Ui {
class SettingsDlg;
}

class SettingsDlg : public QDialog
{
	Q_OBJECT
	
public:
	explicit SettingsDlg(QWidget *parent = 0);
	~SettingsDlg();
	
	void getSettings(Settings &result);
	void setSettings(Settings &value);
	
private:
	Ui::SettingsDlg *ui;
};

#endif // SETTINGSDLG_H
