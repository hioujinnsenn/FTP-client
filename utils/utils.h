//
// Created by ye11 on 2020/2/22.
//

#ifndef WINFTP_UTILS_H
#define WINFTP_UTILS_H

#include<winsock.h>
#include <iostream>
#include <cstring>
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
typedef struct sockaddr_in sockaddr_in;
/*function：发送一条指令，同时收到一条响应回复*/
int SendCommand(SOCKET sock,const char*s,char* parameter);
int SendCommand(SOCKET sock,const char*s);
struct sockaddr_in getLocalAddr(int port);

#endif //WINFTP_UTILS_H
