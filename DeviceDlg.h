#ifndef DEVICEDLG_H
#define DEVICEDLG_H

#include <QDialog>

namespace Ui {
	class DeviceDlg;
}

#include <QSerialPort>
#include <QSerialPortInfo>
#include "model/Device.h"

class DeviceDlg : public QDialog
{
	Q_OBJECT

public:
	explicit DeviceDlg(QWidget *parent = 0);
	~DeviceDlg();
	
	void dialogResult(Device &device);
	void load(Device &device);	

	void update();		
	
	
	
public slots:
	void retranslate();
	
private:
	
	Ui::DeviceDlg *ui;
	
	// QDialog interface
public slots:
	void accept();
};

#endif // DEVICEDLG_H
