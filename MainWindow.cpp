#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QClipboard>
#include <QSerialPortInfo>
#include "DeviceDlg.h"
#include <QSqlQuery>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	setWindowTitle("Easy Terminal");
	
	connect(ui->cmdOpen, SIGNAL(clicked(bool)), this, SLOT(openClicked()));
	connect(ui->actAdd, SIGNAL(triggered(bool)), this, SLOT(addDevice()));
	connect(ui->actEdit, SIGNAL(triggered(bool)), this, SLOT(editDevice()));
	connect(ui->actRemove, SIGNAL(triggered(bool)), this, SLOT(removeDevice()));
	connect(ui->cmbDevices, SIGNAL(currentIndexChanged(int)), this, SLOT(currentDeviceChanged(int)));
	
	ui->txtLog->setEnabled(false);
	
	_port = new QSerialPort("COM5", this);
	
	connect(_port, SIGNAL(readyRead()),this, SLOT(readPort()));

	_deviceDlg = new DeviceDlg(this);
	
	_db = QSqlDatabase::addDatabase("QSQLITE");
	_db.setDatabaseName(qApp->applicationDirPath().append("/db.sqlite"));
	if (!_db.open())
	{
		printLog("Database not found!");
		return;
	}
	
	loadDeviceList();
	
	updateLists();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::readPort()
{
	printLog(_port->readAll());
}

void MainWindow::sendBuffer()
{
//	_port->write(ui->txtInput->text().toLatin1().constData(), 
//				 ui->txtInput->text().length());
}

void MainWindow::printLog(QString text)
{
	ui->txtLog->setPlainText(ui->txtLog->toPlainText().append(text));
	QTextCursor cursor = ui->txtLog->textCursor();
	cursor.movePosition(QTextCursor::End);
	ui->txtLog->setTextCursor(cursor);
}

void MainWindow::sendKey(int key)
{
	if (_port->isOpen())
		_port->putChar(key);
}

void MainWindow::sendKey(QString key)
{
	if (_port->isOpen())
		_port->write(key.toLatin1().constData());
}

void MainWindow::loadDeviceList()
{
	ui->cmbDevices->clear();
	
	QSqlQuery q("select * from devices order by name asc;");
	
	if (!q.first())
		return;
	
	do
	{
		qDebug("%d", q.value("id").toInt());
		ui->cmbDevices->addItem(q.value("name").toString(), q.value("id").toInt());
	}
	while (q.next());

}

void MainWindow::openClicked()
{
	_postpone.clear();
	
	if (_port->isOpen())
	{
		_port->close();
		ui->cmdOpen->setText("Open");
		printLog("\n[Port closed!]\n");
	}
	else
	{
		_port->setBaudRate(_currentDevice.getBaudRate());
		_port->setDataBits(_currentDevice.getDataBits());
		_port->setPortName(ui->cmbPort->currentText());
		_port->setParity(_currentDevice.getParity());
		_port->setStopBits(_currentDevice.getStopBits());
		ui->chkEcho->setChecked(_currentDevice.getEcho());
		
		if (_port->open(QIODevice::ReadWrite))
		{
			printLog("[Port openned!]\n");
		}
		else
		{
			printLog("[Port unavailable!]\n");
			return;
		}
		ui->cmdOpen->setText("Close");
	}
	changeEnableState();
}

void MainWindow::addDevice()
{
	_deviceDlg->update();
	if (_deviceDlg->exec() == QDialog::Accepted) 
	{
		Device result;
		_deviceDlg->dialogResult(result);
		if (!result.insert())
		{
			printLog(tr("Database error! Insertion error!"));
		}
		loadDeviceList();
	}
}

void MainWindow::editDevice()
{
	Device d;
	if (!d.load(ui->cmbDevices->currentData().toInt()))
	{
		printLog(tr("Database error! Device not found!"));
	}
	_deviceDlg->load(d);
	if (_deviceDlg->exec() == QDialog::Accepted) 
	{
		_deviceDlg->dialogResult(d);
		if (!d.update(d.getId()))
		{
			printLog(tr("Database error! Update error!"));
		}
		loadDeviceList();
	}
}

void MainWindow::removeDevice()
{
	Device d;
	if (!d.remove(ui->cmbDevices->currentData().toInt()))
	{
		printLog(tr("Database error! Device not found!"));
	}
	loadDeviceList();
}

void MainWindow::currentDeviceChanged(int index)
{
	Q_UNUSED(index)
	
	if (!_currentDevice.load(ui->cmbDevices->currentData().toInt()))
	{
		printLog("Database failed! Device not found!");
	}
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
	if ((event->type() == QEvent::KeyPress)&&(watched == this))
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		QString text = keyEvent->text();
		
		if (ui->chkCaps->isChecked())
			text = text.toUpper();
		
		if ((keyEvent->key() == Qt::Key_V)&&(keyEvent->modifiers() == Qt::ControlModifier))
		{
			if (ui->chkCaps->isChecked())
				sendKey(qApp->clipboard()->text().toUpper());
			else
				sendKey(qApp->clipboard()->text());
			if (ui->chkEcho->isChecked())
				qApp->clipboard()->text();
		}
		else if ((keyEvent->key() == Qt::Key_C)&&(keyEvent->modifiers() == Qt::AltModifier))
		{
			ui->txtLog->clear();
		}
		else if ((keyEvent->key() == Qt::Key_Enter)||(keyEvent->key() == Qt::Key_Return))
		{
			if (_currentDevice.getPostpone())
			{
				sendKey(_postpone);
				_postpone.clear();
			}
			sendKey(_currentDevice.getLineEnd());
			if (ui->chkEcho->isChecked())
				printLog(text);
		}
		else if (keyEvent->key() == Qt::Key_Backspace)
		{
			if (_currentDevice.getPostpone())
			{
				_postpone.remove(_postpone.length() - 1, 1);
				ui->txtLog->setPlainText(ui->txtLog->toPlainText().remove(ui->txtLog->toPlainText().length() - 1, 1));
			}
			else
			{
				sendKey(_currentDevice.getLineEnd());
				if (ui->chkEcho->isChecked())
					printLog(text);
			}
		}
		else
		{
			if (_currentDevice.getPostpone())
				_postpone.append(text);
			else
				sendKey(text);
			if (ui->chkEcho->isChecked())
				printLog(text);
		}
		return true;
	}
	else
	{
		return QObject::eventFilter(watched, event);
	}
}

void MainWindow::changeEnableState()
{
	ui->cmbPort->setEnabled(!ui->cmbPort->isEnabled());
	ui->txtLog->setEnabled(!ui->txtLog->isEnabled());
}

void MainWindow::updateLists()
{
	ui->cmbPort->clear();
	QList<QSerialPortInfo> spi = QSerialPortInfo::availablePorts();
	foreach (QSerialPortInfo pi, spi) 
	{
		int idx = 0;
		for (int i = 0; i < ui->cmbPort->count(); i++)
		{
			if (ui->cmbPort->itemText(i) > pi.portName())
			{
				idx = i - 1;
				break;
			}
		}
		ui->cmbPort->insertItem(idx, pi.portName());
	}
	
}
