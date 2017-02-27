#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QClipboard>
#include <QSerialPortInfo>
#include "DeviceDlg.h"

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
	
	ui->cmbParity->clear();
	ui->cmbParity->addItem("None", QSerialPort::NoParity);
	ui->cmbParity->addItem("Even", QSerialPort::EvenParity);
	ui->cmbParity->addItem("Odd", QSerialPort::OddParity);
	ui->cmbParity->addItem("Space", QSerialPort::SpaceParity);
	ui->cmbParity->addItem("Mark", QSerialPort::MarkParity);
	ui->cmbParity->setCurrentIndex(0);
	
	ui->cmbStop->clear();
	ui->cmbStop->addItem("1", QSerialPort::OneStop);
	ui->cmbStop->addItem("1.5", QSerialPort::OneAndHalfStop);
	ui->cmbStop->addItem("2", QSerialPort::TwoStop);
	ui->cmbStop->setCurrentIndex(0);

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

void MainWindow::openClicked()
{
	if (_port->isOpen())
	{
		_port->close();
		ui->cmdOpen->setText("Open");
		//updateLists();
		printLog("\n[Port closed!]\n");
	}
	else
	{
		printLog(QString("[Connect to port %1 %2 %3-%4-%5...]\n").arg(
					ui->cmbPort->currentText(),
					ui->cmbBaud->currentText(),
					ui->cmbData->currentText(),
					ui->cmbStop->currentText(),
					ui->cmbParity->currentText())
			   );
		_port->setPortName(ui->cmbPort->currentText());
		_port->setBaudRate(ui->cmbBaud->currentText().toInt());
		_port->setDataBits((QSerialPort::DataBits)ui->cmbData->currentText().toInt());
		_port->setStopBits((QSerialPort::StopBits)ui->cmbStop->currentData().toInt());
		_port->setParity((QSerialPort::Parity)ui->cmbParity->currentData().toInt());
		
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
	DeviceDlg dlg(this);
	
	if (dlg.exec() == QDialog::Accepted) 
	{
		
	}
}

void MainWindow::editDevice()
{
	DeviceDlg dlg(this);
	
	if (dlg.exec() == QDialog::Accepted) 
	{
		
	}
}

void MainWindow::removeDevice()
{
	
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
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
	ui->cmbBaud->setEnabled(!ui->cmbBaud->isEnabled());
	ui->cmbData->setEnabled(!ui->cmbData->isEnabled());
	ui->cmbParity->setEnabled(!ui->cmbParity->isEnabled());
	ui->cmbPort->setEnabled(!ui->cmbPort->isEnabled());
	ui->cmbStop->setEnabled(!ui->cmbStop->isEnabled());
	ui->txtLog->setEnabled(!ui->txtLog->isEnabled());
}

void MainWindow::updateLists()
{
	ui->cmbPort->clear();
	QList<QSerialPortInfo> spi = QSerialPortInfo::availablePorts();
	foreach (QSerialPortInfo pi, spi) 
	{
		ui->cmbPort->addItem(pi.portName());
	}
	
}
