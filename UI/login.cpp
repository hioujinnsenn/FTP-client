#include "login.h"
#include "ui_login.h"
#include "UI/mainwindow.h"
extern string username;
extern string password;
extern string ip;
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    this->setWindowIcon(QIcon("../UI/resoucre/icon/64/ftp.png"));
    ui->setupUi(this);
    ui->lineEdit->setText("127.0.0.1");
    ui->lineEdit_2->setText("jh");
    ui->lineEdit_3->setText("123456");
}

Login::~Login()
{
    delete ui;
}
/**
 * @details 登录按钮槽函数
 *          在输入框中输入数据后，
 * */



void Login::on_pushButton_clicked()
{
       std::string url=ui->lineEdit->text().toStdString();//lineEdit::服务器地址
       username=ui->lineEdit_2->text().toStdString(); //用户名
       password=ui->lineEdit_3->text().toStdString(); //密码
       ip=url;
       char* username_char=(char*)malloc(Dlength);
       char* password_char=(char*)malloc(Dlength);
       char* ip_char=(char*)malloc(Dlength);
       memset(username_char,0,Dlength);
       memset(password_char,0,Dlength);
       memset(ip_char,0,Dlength);
       sprintf(username_char,username.data());
       sprintf(password_char,password.data());
       sprintf(ip_char,url.data());
       SOCKET socket=login(username_char,password_char,ip_char);
       if(socket>0&&socket<65536)
       {
           printf("成功登录，等待进一步操作");
           QString s=QString( "登录成功");
           ui->label_5->setText(s);
           cout.flush();
           CommandSocket=socket;
           login_status=1; //设置登录情况为正常
           this->close();    //关闭登录窗口
       }
       else{
           int ecode=code.getCode();
           if(ecode==-1) {
               ui->label_5->setText("登录失败，请检查账号密码！");
           } else if(ecode==-2) {
               ui->label_5->setText("登录失败，请检查网络及服务器地址！");
           }
           login_status=0;
       }
}
