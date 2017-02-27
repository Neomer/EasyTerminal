#ifndef DEVICEDLG_H
#define DEVICEDLG_H

#include <QDialog>

namespace Ui {
class DeviceDlg;
}

class DeviceDlg : public QDialog
{
	Q_OBJECT
	
public:
	explicit DeviceDlg(QWidget *parent = 0);
	~DeviceDlg();
	
private:
	
	
private:
	Ui::DeviceDlg *ui;
};

#endif // DEVICEDLG_H
