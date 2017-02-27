#include "DeviceDlg.h"
#include "ui_DeviceDlg.h"

DeviceDlg::DeviceDlg(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DeviceDlg)
{
	ui->setupUi(this);
	
	connect(ui->cmdOK, SIGNAL(clicked(bool)), this, SLOT(accept()));
	connect(ui->cmdCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
	
}

DeviceDlg::~DeviceDlg()
{
	delete ui;
}
