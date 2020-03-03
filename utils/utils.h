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
#include <sstream>
//引入windows的文件夹API
#include <shlwapi.h>
#include <queue>
using  namespace  std;
#define PASV  "PASV\r\n"
#define CWD   "CWD %s\r\n"
#define MKD    "MKD %s\r\n"
#define RETR  "RETR %s\r\n"
#define STOR  "STOR %s\r\n"
//rest指令是实现断点续传的关键
#define REST  "REST %ld\r\n"
#define QUIT  "QUIT\r\n"
#define SIZE  "SIZE %s\r\n"
#define PASS  "PASS %s\r\n"
#define USER  "USER %s\r\n"
#define PWD   "PWD\r\n"
//注意List命令需要走数据接口
#define LIST  "LIST %s\r\n"
#define LISTCUR  "LIST\r\n"
#define clength 400
#define Dlength 4000
/*file：类似于linux的操作，将文件和目录都视为File*/

typedef struct File{
    int     type;/*1:目录，2：文件*/
    string  name;/*文件、目录名称*/
    string  path;/*完整的文件路径*/
    long     size;/*以字节为单位，目录大小设置为0，文件目录根据实际大小确定*/
}File;

typedef struct sockaddr_in sockaddr_in;

/*function：发送一条指令，同时收到一条响应回复*/
string SendCommand(SOCKET sock,const char*s, char* parameter);
string SendCommand(SOCKET sock,const char*s);
string SendCommand(SOCKET sock, const char*s,long size);

/**
 * @details 返回本地地址，简单设置
 * */
struct sockaddr_in getLocalAddr(int port);

/**
 * @details ls命令，无路径参数
 * */
vector<File> ls(SOCKET sock,SOCKET DataSock);

/**
 * @details ls命令，带路径参数
 * */
vector<File> ls(SOCKET sock,SOCKET datasock,string dir);

/**
 * @details 返回当前路径pwd
 *          注意pwd命令的返回信息就在命令端口，无需前往数据端口
 * */
string pwd(SOCKET sock);

/**
 * @details 路径切割成一个个独立的文件路径和信息
 * */
vector<File> split_dir(string dirs,string parent);

File deal_file_item(string p,string parent);

string cwd(SOCKET  sock,string path);

long size(SOCKET sock,string filepath);

int mkdir_local(string dir);
#endif //WINFTP_UTILS_H
