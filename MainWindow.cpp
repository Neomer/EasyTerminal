#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QClipboard>
#include <QSerialPortInfo>
#include "DeviceDlg.h"
#include "model/Device.h"
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
	if (_port->isOpen())
	{
		_port->close();
		ui->cmdOpen->setText("Open");
		printLog("\n[Port closed!]\n");
	}
	else
	{
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
	qDebug("%d", ui->cmbDevices->currentData().toInt());
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
	
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
	if ((event->type() == QEvent::KeyPress)&&(watched == this))
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if ((keyEvent->key() == Qt::Key_V)&&(keyEvent->modifiers() == Qt::ControlModifier))
		{
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
			sendKey('\r');
			sendKey('\n');
			if (ui->chkEcho->isChecked())
				printLog(keyEvent->text());
		}
		else
		{
			sendKey(keyEvent->text());
			if (ui->chkEcho->isChecked())
				printLog(keyEvent->text());
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
