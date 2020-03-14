#include "UI/mainwindow.h"
#include <QApplication>
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


SOCKET CommandSocket;  //全局的socket对象
ErrorCode code;
/**
 * @deprecated 弃用，仅作为一开始摸索使用，已弃用
 * @brief 连接到本地ftp服务器
 * @author 叶茂鑫
 * @details 连接到本机的127.0.0.1的21端口的ftp
 *          该服务端口暂时设置为匿名可访问
 *          根据由IBM教程中获得的关于ftp协议的规定内容设置，只要实现对应的即可
 *          此处先进行ftp连接和认证
 * */
int Connect_to_Server(){
    WSADATA wsadata;
    WORD minVersionRequired;
    minVersionRequired=MAKEWORD(1,1);         //生成版本号
    int err=WSAStartup(minVersionRequired,&wsadata);   //启动wsa的服务
    if(err!=0)
    {
        return -1 ;
    }
    //对版本要求的低字节和高字节进行要求
    if(LOBYTE(wsadata.wVersion)!=1||HIBYTE(wsadata.wVersion)!=1)
    {
        WSACleanup();
        return -1;
    }

    SOCKET sock=socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in ip_mem;
    int error;
    int slen= sizeof(ip_mem);
    memset(&ip_mem, 0,sizeof(ip_mem)); //设置sockaddr的变量的指针为原零
    ip_mem.sin_family=AF_INET;
    ip_mem.sin_port=htons(21);
    ip_mem.sin_addr.s_addr=inet_addr("127.0.0.1");
    //为何将sockaddr_in转sockaddr，尽管有说明就是了
    int err_connect=connect(sock,(struct sockaddr *)&ip_mem, sizeof(ip_mem));
    if(err_connect==0)
    {
        char* message=(char*)malloc(sizeof(int)*100);
        recv(sock,message,400,0);
        std::string sm=message;
        std::cout<<sm;
    } else{
        std::cout<<"连接失败";
    }
    std::cout.flush();
    //首次握手完毕，获得ftp服务器的欢迎信息
     char *command=(char *)malloc(clength);
     char *message=(char *)malloc(clength);
     memset(command,0, clength);
     string Message;
     sprintf(command,"USER jh\r\n");
     send(sock,command,clength,0);
     recv(sock,message,clength,0);
     Message=message;
     cout<<Message;
     sprintf(command,"PASS 123456\r\n");
     send(sock,command,clength,0);
     recv(sock,message,clength,0);
     Message=message;
     cout<<Message;


}


int main(int argc, char *argv[])
{
   //开始编写界面逻辑测试
   //一个应用程序似乎只可以有一个MainWindow
   QApplication app(argc,argv);
   MainWindow  mainWindow;
   mainWindow.show();
   return app.exec();
}
