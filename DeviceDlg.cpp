#include "DeviceDlg.h"
#include "ui_DeviceDlg.h"
#include <QMessageBox>

DeviceDlg::DeviceDlg(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DeviceDlg)
{
	ui->setupUi(this);
	
	connect(ui->cmdOK, SIGNAL(clicked(bool)), this, SLOT(accept()));
	connect(ui->cmdCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));

	update();	
}

DeviceDlg::~DeviceDlg()
{
	delete ui;
}

void DeviceDlg::dialogResult(Device &device)
{
	device.setBaudRate(ui->baudRateComboBox->currentText().toInt());
	device.setDataBits(ui->dataBitsComboBox->currentData().value<QSerialPort::DataBits>());
	device.setEcho(ui->echoCheckBox->isChecked());
	device.setLineEnd(ui->lineEndComboBox->currentData().toString());
	device.setName(ui->deviceNameLineEdit->text());
	device.setParity(ui->parityComboBox->currentData().value<QSerialPort::Parity>());
	device.setPostpone(ui->postponedWritingCheckBox->isChecked());
	device.setStopBits(ui->stopBitsComboBox->currentData().value<QSerialPort::StopBits>());
	device.setCaps(ui->capsCheckBox->isChecked());
}

void DeviceDlg::load(Device &device)
{
	update();

	ui->deviceNameLineEdit->setText(device.getName());
	ui->echoCheckBox->setChecked(device.getEcho());
	ui->postponedWritingCheckBox->setChecked(device.getPostpone());
	ui->capsCheckBox->setChecked(device.getCaps());
	for (int i = 0; i < ui->baudRateComboBox->count(); i++)
	{
		if (ui->baudRateComboBox->itemText(i).toInt() == device.getBaudRate())
		{
			ui->baudRateComboBox->setCurrentIndex(i);
			break;
		}
	}
	for (int i = 0; i < ui->parityComboBox->count(); i++)
	{
		if (ui->parityComboBox->itemData(i).value<QSerialPort::Parity>() == device.getParity())
		{
			ui->parityComboBox->setCurrentIndex(i);
			break;
		}
	}
	for (int i = 0; i < ui->stopBitsComboBox->count(); i++)
	{
		if (ui->stopBitsComboBox->itemData(i).value<QSerialPort::StopBits>() == device.getStopBits())
		{
			ui->stopBitsComboBox->setCurrentIndex(i);
			break;
		}
	}
	for (int i = 0; i < ui->dataBitsComboBox->count(); i++)
	{
		if (ui->dataBitsComboBox->itemData(i).value<QSerialPort::DataBits>() == device.getDataBits())
		{
			ui->dataBitsComboBox->setCurrentIndex(i);
			break;
		}
	}
	for (int i = 0; i < ui->lineEndComboBox->count(); i++)
	{
		if (ui->lineEndComboBox->itemData(i).toString() == device.getLineEnd())
		{
			ui->lineEndComboBox->setCurrentIndex(i);
			break;
		}
	}
}

void DeviceDlg::accept()
{
	if (ui->deviceNameLineEdit->text().isEmpty())
	{
		QMessageBox::critical(this, tr("Error"), tr("You must enter device name!"));
		return;
	}
	
	QDialog::accept();
}

void DeviceDlg::update()
{
	ui->deviceNameLineEdit->clear();
	
	ui->parityComboBox->clear();
	ui->parityComboBox->addItem(tr("None"), QSerialPort::NoParity);
	ui->parityComboBox->addItem(tr("Even"), QSerialPort::EvenParity);
	ui->parityComboBox->addItem(tr("Odd"), QSerialPort::OddParity);
	ui->parityComboBox->addItem(tr("Space"), QSerialPort::SpaceParity);
	ui->parityComboBox->addItem(tr("Mark"), QSerialPort::MarkParity);
	ui->parityComboBox->setCurrentIndex(0);
	
	ui->stopBitsComboBox->clear();
	ui->stopBitsComboBox->addItem("1", QSerialPort::OneStop);
	ui->stopBitsComboBox->addItem("1.5", QSerialPort::OneAndHalfStop);
	ui->stopBitsComboBox->addItem("2", QSerialPort::TwoStop);
	ui->stopBitsComboBox->setCurrentIndex(0);
	
	ui->baudRateComboBox->clear();
	foreach (int baud, QSerialPortInfo::standardBaudRates())
	{
		ui->baudRateComboBox->addItem(QString::number(baud), baud);
	}
	ui->baudRateComboBox->setCurrentIndex(0);
	
	ui->dataBitsComboBox->clear();
	ui->dataBitsComboBox->addItem("5", QSerialPort::Data5);
	ui->dataBitsComboBox->addItem("6", QSerialPort::Data6);
	ui->dataBitsComboBox->addItem("7", QSerialPort::Data7);
	ui->dataBitsComboBox->addItem("8", QSerialPort::Data8);
	ui->dataBitsComboBox->setCurrentIndex(0);	
	
	ui->lineEndComboBox->clear();
	ui->lineEndComboBox->addItem("<CR>", "\r");
	ui->lineEndComboBox->addItem("<LF>", "\n");
	ui->lineEndComboBox->addItem("<CR><LF>", "\r\n");
	ui->lineEndComboBox->setCurrentIndex(0);
	
}

void DeviceDlg::retranslate()
{
	ui->retranslateUi(this);
}
