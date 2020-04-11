//
// Created by asus on 2020/2/27.
//

#include "commands.h"
#include <QFileInfo>
#include <QtCore/QDir>
#include <QDebug>
#include <QtCore/QDirIterator>

/***
 * 进入被动模式，返回数据端口socket。每次调用后记得手动关闭打开的数据端口
 * @param sock  控制端口
 * @return  数据端口
 */
SOCKET pasv(SOCKET sock)
{
    string message=SendCommand(sock, PASV);    //开启被动
    string port=message;
    int newport=getNewPort(port);
    cout<<newport<<endl;
    SOCKET  DataSock=getNewSocket("127.0.0.1",newport);
    return DataSock;
}

/***
 * 关闭数据端口，返回控制端口收到的信息
 * 弃用，应该使用quit命令
 * @param sock
 * @param dataSock
 * @return
 */
string closeDataSock(SOCKET sock, SOCKET dataSock)   //说实话，直接用quit命令就好了
{
    char* message=(char*)malloc(clength);
    memset(message, 0, clength);
    closesocket(dataSock);
    recv(sock, message, clength, 0);
    return message;
}

/***
 * 输入本地文件路径，得到该本地文件的大小。返回类型为size_t（无符号64位整数），单位B
 * @param filePath 文件路径
 * @return 文件大小，单位B，类型size_t
 */
size_t getFileSize(string filePath)    //得到本地文件的大小
{

    QString fileQstr=QString(filePath.data());
    QFileInfo info=QFileInfo(fileQstr);
    if( info.exists())         //如果文件存在才返回数据
    {
        size_t size= info.size();
        return size;
    } else{
        return 1;
    }
}

/***
 * 将size返回的文件大小数据由字符串转为size_t类型
 * @param s 字符串类型的文件大小，单位B
 * @return size_t类型的文件大小，单位B，十进制
 */
size_t stringToSize_t(string s)
{
    size_t temp;
    return stoul(s, &temp);     //将string转换为size_t类型，最后的参数为设置进制，默认为10。temp存储的是字符串内第一个非数字字符的序号。（序号从0始）
}

/***
 * 递归得到指定的本地目录下所有文件的名字和绝对路径信息，返回结果由两个引用传递
 * @param dirPath 目录的本地路径
 * @param files 引用，得到并记录目录下所有的文件路径
 * @param names 引用，得到并记录目录下所有的文件名（用于简化字符串运算，而不需要裁剪得到文件名）
 */
void getAllFiles(string dirPath, vector<string> &files, vector<string> &names)  //遍历文件夹中的所有文件（包括文件夹本身）
{
    QDir dir(QString::fromStdString(dirPath));
    if(!dir.exists())
    {
        qDebug()<<"it is not true dir_path";
    }
    //迭代器遍历目录，设置过滤参数，QDir::NoDotAndDotDot表示不会去遍历上层目录
    QDirIterator iterator(QString::fromStdString(dirPath), QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while(iterator.hasNext()){
        iterator.next().toStdString();
        string fileName=iterator.fileName().toStdString();
        QFileInfo info=iterator.fileInfo();
        string filePath=info.absoluteFilePath().toStdString();  //获取文件绝对路径
        files.push_back(filePath);
        names.push_back(fileName);
    }
}

/***
 * 在服务器创建目录（最好为绝对路径，比较稳妥）
 * @param sock
 * @param path 所需创建的路径
 * @return 成功创建开头前三位返回250，否则数字 > 300
 */
string mkd(SOCKET sock,string path ){  //在服务器创建目录

    char* p=(char*)malloc(clength);
    memset(p, 0, clength);
    sprintf(p, path.c_str());
    string r=SendCommand(sock, MKD, p);
    return r;
}