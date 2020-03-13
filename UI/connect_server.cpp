#include "connect_server.h"
#include "ui_connect_server.h"

connect_server::connect_server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connect_server)
{
    ui->setupUi(this);
}

connect_server::~connect_server()
{
    delete ui;
}
