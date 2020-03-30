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
    char*command=(char*)malloc(clength);
    memset(command,0,clength);
    char*message=(char*)malloc(clength);
    memset(message,0,clength);
    string Message;
    sprintf(command,s,parameter);
    send(sock,command, clength,0);
    recv(sock,message, clength,0);
    Message=message;
    cout<<Message;
    free(command);
    free(message);
    return Message;
}
string SendCommand(SOCKET sock,const char*s,long size)
{
    char*command=(char*)malloc(clength);
    memset(command,0,clength);
    char*message=(char*)malloc(clength);
    memset(message,0,clength);
    string Message;
    sprintf(command,s,size);
    send(sock,command, clength,0);
    recv(sock,message, clength,0);
    Message=message;
    cout<<Message;
    free(command);
    free(message);
    return Message;
}
/*不需要多余参数的就用这个*/
string SendCommand(SOCKET sock,const char*s)
{
    char*command=(char*)malloc(clength);
    memset(command,0,clength);
    char*message=(char*)malloc(clength);
    memset(message,0,clength);
    string Message;
    sprintf(command,s);
    send(sock,command, clength,0);
    recv(sock,message, clength,0);
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
vector<File> ls(SOCKET sock,SOCKET DataSock)
{
    char* message=(char*)malloc(Dlength);
    memset(message,0,Dlength);
    char* resmessage=(char*)malloc(Dlength);
    SendCommand(sock,LISTCUR);
    recv(sock,resmessage,Dlength,0);
    recv(DataSock,message,Dlength,0);
    string p;
    p=message;
    //测试输出
    cout<<p;
    free(message);
    string parent=pwd(sock);
    return split_dir(p,parent);
}

/**
 * @details ls命令，带路径参数
 * */
vector<File> ls(SOCKET sock,SOCKET datasock,string dir)
{
    char* message=(char*)malloc(Dlength);
    memset(message,0,Dlength);
    char* path=(char*)malloc(Dlength);
    memset(path,0,Dlength);
    sprintf(path,dir.data());
    SendCommand(sock,LIST,path);
    //ls后台会产生两句返回信息，应读取完毕
    //SendCommand只默认读取一次
    recv(datasock,message,Dlength,0);
    char*  command_successBack=(char*)malloc(Dlength);
    //处理返回信息
    recv(sock,command_successBack,Dlength,0);
    string p=message;
    //测试输出
    free(message);
    free(path);
    return  split_dir(p,dir);
}

/**
 * @details 为了不复杂化接口的设计，此处的设计中，文件路径不加上绝对路径
 *           否则还得加上一个pwd命令，需要对应的sock参数。
 * */
vector<File> split_dir(string dirs,string parent){
    vector<File> files;
    int i=0;
    while(( i=dirs.find_first_of("\r\n"))!=-1)
    {
        //此处正好是从0，开始前i-1个，碰巧没遇上问题
        //如果pos不是0，就需要注意第二个参数是截取的长度。
       string p=dirs.substr(0,i);
       File f=deal_file_item(p,parent);
       files.push_back(f);
       dirs=dirs.substr(i+2);
    }
    return files;
}
/**
 * 解析一条文件信息时，一个分为九个部分，
 * 每个部分之间用空格隔开，要求注意目录、文件名可能含有空格
 * 因此需要由前向后处理，不可从后面直接用空格截取，会只取到一小部分的文件名
 * //TODO 此处暂时只提取三个信息，目录或者文件的类别信息，，文件大小，文件名信息
 * */
File deal_file_item(string p,string parent)
{
    File f;
    f.parent=parent;
    if(p[0]=='d')
    {
        f.type=1;//目录文件
    }
    else{
        f.type=2;//普通文件
    }
    int i=0;
    while(i<8)
    {
        int  j=p.find_first_of(' ');
        string p1=p.substr(0,j);
        p=p.substr(j+1);
        while(p[0]==' ')
        {
            p=p.substr(1);
        }
        switch (i)
        {
            case 4:{

                //stoi不是标准函数，慎用，用流的转化就可以了
                istringstream is(p1);
                is>>f.size;
                //任何文件都计算大小，但是只有普通文件到时候才显示大小。
            };break;
            case 7:{
                //第八次分割时，文件名在最后剩下的这一部分，但这一部分可能含有空格
                stringstream path("");
                path<<parent;

                //只有根路径时才以/结尾，其余目录均不会以/结尾，拼接路径时需要加上去
                if(parent.size()>0&&parent[parent.size()-1]!='/')
                {
                    path<<'/';
                }
                path<<p;
                f.path=path.str();
                f.name=p;
            };break;
        }
        i++;

    }
    return f;
}
/**
 * @details 返回当前路径pwd
 *          注意pwd命令的返回信息就在命令端口，无需前往数据端口
 * */
string pwd(SOCKET sock)
{
    string p=SendCommand(sock,PWD);
    int i=p.find_first_of('\"');
    int j=p.find_last_of('\"');
    //注意substr的参数问题
    return p.substr(i+1,(j-i-1));
}

string cwd(SOCKET sock,string path)
{
    char* p=(char*)malloc(Dlength);
    memset(p,0,Dlength);
    sprintf(p,path.data());
    string message=  SendCommand(sock,CWD,p);  //只处理掉一条信息
    free(p);
    return message;
}

long size(SOCKET sock,string filepath){

    char*  path=(char*)malloc(Dlength);
    memset(path,0,Dlength);
    sprintf(path,filepath.data());
    string p=SendCommand(sock,SIZE,path);
    int i=p.find_last_of(' ');
    string size=p.substr(i+1,(p.size()-i-3));  //p.size()-2 -(i+1)
    istringstream  is(size);
    long sizel;
    is>>sizel;
    free(path);
    return sizel;
}

int mkdir_local(string dir)
{
    if(!PathIsDirectory(dir.data()))  //如果目录不存在
    {
        CreateDirectory(dir.data(),NULL);
    }
    return 0;
}

int rename(SOCKET  sock,string oldname,string newname){
    char* old=(char*)malloc(Dlength);
    char* newName=(char*)malloc(Dlength);
    memset(old,0,Dlength);
    memset(old,0,Dlength);
    sprintf(old,oldname.data());
    sprintf(newName,newname.data());
    SendCommand(sock,RNFR,old);
    SendCommand(sock,RNTO,newName);
    free(old);
    free(newName);

}

int del(SOCKET sock,string name){
    char* message=(char*)malloc(Dlength);
    memset(message,0,Dlength);
    sprintf(message,name.data());
    SendCommand(sock,DELE,message);
    free(message);
    return 0;
}
int rmd(SOCKET sock,string path){
    char* message=(char*)malloc(Dlength);
    memset(message,0,Dlength);
    sprintf(message,path.data());
    SendCommand(sock,RMD,message);
    free(message);
    return 0;
}
