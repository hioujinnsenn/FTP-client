#include "login.h"
#include "ui_login.h"
#include "UI/mainwindow.h"
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}
/**
 * @details 登录按钮槽函数
 *          在输入框中输入数据后，
 * */

