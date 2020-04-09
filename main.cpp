#include "UI/mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <winsock.h>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <ftpsock/login.h>
#include <ftpsock/ftpdata.h>
#include <utils/utils.h>
#include "ftpsock/upload.h"
#include "ftpsock/download.h"
#include "UI/login.h"
#include "UI/mainwindow.h"
#include "utils/ErrorCode.h"
#define clength 400
#define dlength 1000
using namespace std;

int login_status=0;
SOCKET CommandSocket;  //全局的socket对象
ErrorCode code;
vector<File> localFiles;
string username;
string  password;
string ip;
/**
 * @details 打开时事先获取本地的文件列表，加快处理速度
 *
 * */
int main(int argc, char *argv[])
{
   //开始编写界面逻辑测试
   //一个应用程序似乎只可以有一个MainWindow
   QApplication app(argc,argv);
   QDir dir("c:/");

   QFileInfoList list=dir.entryInfoList();
    for(int i=0;i<list.size();i++)
    {
        File file;
        if(list.operator[](i).isFile())
        {
            file.size=list.operator[](i).size();
            file.name=list.operator[](i).fileName().toStdString();
            file.path=list.operator[](i).absoluteFilePath().toStdString();
            file.type=2; //普通文件
        }
        else{
            file.path=list.operator[](i).absoluteFilePath().toStdString();
            file.name=list.operator[](i).fileName().toStdString();
            file.type=1; //目录文件
        }
        localFiles.push_back(file);
    }
   Login login1;
   login1.show();
   app.exec();
   if(login_status==1)    //登录成功
   {
       MainWindow mainWindow;
       mainWindow.local_pwd="c:/";
       mainWindow.show();
       app.exec(); //可以再次使用app.exec进行重新执行
   }
   return 0;
}


