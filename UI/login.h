#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "ftpsock/login.h"
#include "utils/utils.h"
#include "utils/ErrorCode.h"
extern SOCKET CommandSocket;   //外部引入的变量
extern int login_status;
namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();


private slots:
    void on_pushButton_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
