#include "ftpmain.h"
#include "ui_ftpmain.h"

FtpMain::FtpMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FtpMain)
{
    ui->setupUi(this);
}

FtpMain::~FtpMain()
{
    delete ui;
}
