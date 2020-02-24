//
// Created by ye11 on 2020/2/22.
//

#include "utils.h"
using namespace std;

/**
 * 使用注意
 * 使用此函数的要点在于，s参数必须使用设置好的预定义宏
 * 而且必须有参数parameter需要填；
 * @return 回显服务器的回复
 * #define SIZE  "SIZE %s\r\n"
   #define PASS  "PASS %s\r\n"
   #define USER  "USER %s\r\n"
   #define CWD   "CWD %s\r\n"
   #define RETR  "RETR %s\r\n"
   #define STOR  "STOR %s\r\n"
   //rest指令是实现断点续传的关键
   #define REST  "REST %s\r\n"
   #define LIST  "LIST %s\r\n"
 * */

string SendCommand(SOCKET sock,const char*s,char* parameter)
{
    char*command=(char*)malloc(length);
    memset(command,0,length);
    char*message=(char*)malloc(length);
    memset(message,0,length);
    string Message;
    sprintf(command,s,parameter);
    send(sock,command, length,0);
    recv(sock,message, length,0);
    Message=message;
    cout<<Message;
    free(command);
    free(message);
    return Message;
}

/*不需要多余参数的就用这个*/
string SendCommand(SOCKET sock,const char*s)
{
    char*command=(char*)malloc(length);
    memset(command,0,length);
    char*message=(char*)malloc(length);
    memset(message,0,length);
    string Message;
    sprintf(command,s);
    send(sock,command, length,0);
    recv(sock,message, length,0);
    Message=message;
    cout<<Message;
    free(command);
    free(message);
    return Message;
}
struct sockaddr_in getLocalAddr(int port){
    struct sockaddr_in  addr;
    memset(&addr,0, sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    return addr;
}

/**
 * @details ls命令，不带路径参数
 * */
string ls(SOCKET sock,SOCKET DataSock)
{
    char* message=(char*)malloc(Dlength);
    memset(message,0,Dlength);
    SendCommand(sock,LISTCUR);
    recv(DataSock,message,Dlength,0);
    string p=message;
    free(message);
    //处理成string将每个文件、文件夹独立出
    return p;
}

/**
 * @details ls命令，带路径参数
 * */
string ls(SOCKET sock,SOCKET datasock,char* dir)
{
    char* message=(char*)malloc(Dlength);
    memset(message,0,Dlength);
    SendCommand(sock,LIST,dir);
    recv(datasock,message,Dlength,0);
    string p=message;
    free(message);
    return  p;
}