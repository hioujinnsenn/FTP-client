//
// Created by ye11 on 2020/2/22.
//

#include "login.h"

using namespace std;
SOCKET login(char* username,char* password)
{

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
    char* command=(char*)malloc(clength);
    char* message=(char*)malloc(clength);
    memset(message,0,clength);
    memset(command,0,clength);
    string  Message;
    sprintf(command,"USER %s\r\n",username);
    send(sock,command,clength,0);
    recv(sock,message,clength,0);
    Message=message;
    cout<<Message;
    //memset和sprintf一定要严格的搭配好，先清楚上一次残留的数据，再写入新的数据
    memset(message,0,clength);
    memset(command,0,clength);
    sprintf(command,"PASS %s\r\n",password);
    send(sock,command,clength,0);
    recv(sock,message,clength,0);
    Message=message;
    cout<<Message;
    free(command);
    free(message);
    return sock;
}