//
// Created by asus on 2020/2/24.
//
#include "upload.h"
using namespace std;

/***
 * 上传功能函数，上传本地文件到服务器指定路径位置
 * @param sock
 * @param dirName
 * @param filePath
 * @return 执行的结果以字符串形式返回并打印
 */
string upload(SOCKET sock, string dirName, string filePath)     //上传文件到服务器
{
    ifstream file(filePath);  //需要上传的文件流
    if(! file.is_open()){
        cout<<"文件打开失败！"<<endl;
        return 0;
    }
    SOCKET dataSocket=pasv(sock);   //开启被动模式，返回数据端口socket
    char* fileName=(char*)malloc(filePath.size()); //所需上传文件的文件名
    memset(fileName, 0,filePath.size());

    SendCommand(sock, "TYPE i\r\n");
    int i=(filePath.find_last_of('\\')!=string::npos) ? filePath.find_last_of('\\') : filePath.find_last_of('/');  //找到文件名前的分隔符
    sprintf(fileName, filePath.substr(i+1, string::npos).c_str());  //得到所需上传的文件名

    if(! dirName.empty()){    //若没有指定目录，则不需要change directory
        char* dn=(char*)malloc(dirName.size());
        memset(dn, 0, dirName.size());
        sprintf(dn, dirName.c_str());
        cwd(sock, dn);   //改变工作目录
        free(dn);
    }
    string size=SendCommand(sock,SIZE, fileName);   //用size命令判断要上传的文件是否已经在目录中
    if(size.substr(0, 3)=="213"){
        string r="文件已存在或重名！";
        cout<<r<<endl;
        free(fileName);
        file.close();
        return r;
    }
    string mes=SendCommand(sock, STOR, fileName);  //请求上传文件
    string m=mes.substr(0,2);
    if(m>"300" || m.empty()){
        cout<<"文件上传失败"<<endl;
        file.close();
        free(fileName);
        return "文件上传失败";   //失败返回
    }
    while(! file.eof()){    //在到达文件末尾前持续读文件，将文件内容通过数据端口上传到服务器
        char* message=(char*)malloc(Dlength);   //dataBuffer
        memset(message, 0, Dlength);
        file.read(message, Dlength);
        send(dataSocket, message, Dlength, 0);
        free(message);
    }
    file.close();   //清理现场
    free(fileName);
    string message=closeDataSock(sock, dataSocket);     //关闭数据端口
    cout<<message;
    return "文件上传成功";
}