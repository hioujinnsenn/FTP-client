//
// Created by ye11 on 2020/2/22.
//

#ifndef WINFTP_UTILS_H
#define WINFTP_UTILS_H

#include<winsock.h>
#include <iostream>
#include <cstring>
#include <iostream>
#include  <vector>
using  namespace  std;
#define PASV  "PASV\r\n"
#define CWD   "CWD %s\r\n"
#define RETR  "RETR %s\r\n"
#define STOR  "STOR %s\r\n"
//rest指令是实现断点续传的关键
#define REST  "REST %s\r\n"
#define QUIT  "QUIT\r\n"
#define SIZE  "SIZE %s\r\n"
#define PASS  "PASS %s\r\n"
#define USER  "USER %s\r\n"
//注意List命令需要走数据接口
#define LIST  "LIST %s\r\n"
#define LISTCUR  "LIST\r\n"
#define length 400
#define Dlength 4000
/*file：类似于linux的操作，将文件和目录都视为File*/
typedef struct File{
    int     type;/*1:目录，2：文件*/
    string  name;/*文件、目录名称*/
    string  path;/*完整的文件路径*/
    int     size;/*以字节为单位，目录大小设置为0，文件目录根据实际大小确定*/
}File;
typedef struct sockaddr_in sockaddr_in;

/*function：发送一条指令，同时收到一条响应回复*/
string SendCommand(SOCKET sock,const char*s,char* parameter);
string SendCommand(SOCKET sock,const char*s);

/**
 * @details 返回本地地址，简单设置
 * */
struct sockaddr_in getLocalAddr(int port);

/**
 * @details ls命令，无路径参数
 * */
string ls(SOCKET sock,SOCKET DataSock);

/**
 * @details ls命令，带路径参数
 * */
string ls(SOCKET sock,SOCKET datasock,char * dir);

/**
 * @details 路径切割成一个个独立的文件路径和信息
 * */
vector<string> split_dir(string dirs);
#endif //WINFTP_UTILS_H
