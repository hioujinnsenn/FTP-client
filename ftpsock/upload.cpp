//
// Created by asus on 2020/2/24.
//
#include "upload.h"
using namespace std;

int upload(SOCKET sock, SOCKET dataSocket, string dirName, string filePath)     //上传文件到服务器
{
    char* fileName=(char*)malloc(sizeof(filePath)); //所需上传文件的文件名
    memset(fileName, 0, sizeof(filePath));
    ifstream file;  //需要上传的文件流
    char* dn=(char*)malloc(sizeof(dirName));
    memset(dn, 0, sizeof());

    int i=(filePath.find_last_of('\\')!=string::npos) ? filePath.find_last_of('\\') : filePath.find_last_of('/');  //找到文件名前的分隔符
    sprintf(fileName, filePath.substr(i, string::npos).c_str());  //得到所需上传的文件名
    SendCommand(sock, CWD, dirName.c_str());   //改变工作目录
    string mes=SendCommand(sock, STOR, fileName);  //请求上传文件
    string m=mes.substr(0,2);
    if(m!="150"){
        cout<<"文件上传失败"<<endl;
        return 0;   //失败返回
    }
    file.open(filePath);
    if(! file.is_open()){
        cout<<"文件打开失败！"<<endl;
        return 0;
    }
    while(! file.eof()){    //在到达文件末尾前持续读文件，将文件内容通过数据端口上传到服务器
        char* message=(char*)malloc(Dlength);   //dataBuffer
        file.getline(message, Dlength);
        send(dataSocket, message, Dlength, 0);
        free(message);
    }
    file.close();
    free(fileName);
    return 1;
}