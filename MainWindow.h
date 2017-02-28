#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSqlDatabase>
#include <DeviceDlg.h>
#include "model/Device.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
public slots:
	void readPort();
	void sendBuffer();
	
private:
	void printLog(QString text);
	void sendKey(int key);
	void sendKey(QString key);
	void loadDeviceList();
		
private slots:
	void openClicked();
	void addDevice();
	void editDevice();
	void removeDevice();
	void currentDeviceChanged(int index);
	
protected:
	bool eventFilter(QObject *watched, QEvent *event);
	
private:
	void changeEnableState();
	void updateLists();
	
	Ui::MainWindow *ui;
	QSerialPort *_port;
	QSqlDatabase _db;
	DeviceDlg *_deviceDlg;
	Device _currentDevice;
	QString _postpone, _postpone_last;
};

#endif // MAINWINDOW_H
