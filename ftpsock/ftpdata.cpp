//
// Created by ye11 on 2020/2/22.
//

#include "ftpdata.h"
#include <utils/utils.h>
using namespace std;
#define length 400

/**
 * 返回一个与预设地址端口的socket建立连接的本地socket
 * @param addr:对方的地址设定为const char*：可以直接提供用引号引起来的字符串，也可以提供char*
 * @param port：对方的端口
 * 开启一个新的Socket链接,只需要提供ip地址和端口即可
 * 主要用于数据传输端口的建立和目标网络地址的链接。
 * */
SOCKET getNewSocket(const char* addr,int port){
    struct sockaddr_in sockaddrIn;
    memset(&sockaddrIn,0,sizeof(sockaddrIn));

    //    设置链接地址
    sockaddrIn.sin_family=AF_INET;
    sockaddrIn.sin_port=htons(port);
    sockaddrIn.sin_addr.s_addr=inet_addr(addr);

    SOCKET sock=socket(AF_INET,SOCK_STREAM,0);
    connect(sock,(struct sockaddr*)&sockaddrIn, sizeof(sockaddrIn));
    int error_connnet=WSAGetLastError();
    if(error_connnet==0)      //没有错误发生
    {
        cout<<"新建一个数据socket\n";
        return sock;
    } else{
        cout<<"Error Connect Happen!";
        return -1;
    }
}

/* *
 * 根据PASV命令得到的响应信息，提取出新的端口号码
 * 主要在于字符串切割，比较复杂所以独立抽出
 * */
int  getNewPort(string port)
{
    int i=port.find_first_of('(');
    int j=port.find_last_of(',');
    int k=port.find_first_of(')');
    string ip_rest=port.substr(i,(j-i));
    int m=ip_rest.find_last_of(',');
    string port2=port.substr(j+1,(k-j-1));
    string port1=ip_rest.substr(m+1);
    int numh=stoi(port1);
    int numl=stoi(port2);
    return numh*256+numl;
}