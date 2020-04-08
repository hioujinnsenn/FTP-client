//
// Created by asus on 2020/2/27.
//

#include "commands.h"

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
 * @param sock
 * @param dataSock
 * @return
 */
string closeDataSock(SOCKET sock, SOCKET dataSock)
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
    ifstream file(filePath);
    file.seekg(0, ios::end);
    size_t size=file.tellg();
    file.close();
    return size;
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
    WIN32_FIND_DATA findData;
    HANDLE hFile;   //句柄
    string path(dirPath);   //用于存储当前所在目录的string
    path.append("\\*.*");   //末尾加上通配符用于遍历查找
    hFile=FindFirstFile(path.c_str(), &findData);   //找到第一个文件
    do{
        if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){     //若第一个子文件为目录
            if(strcmp(findData.cFileName, ".")!=0 && strcmp(findData.cFileName, "..")!=0)  //且不为 . 和..（即当前目录和父目录）
                getAllFiles(path.assign(dirPath).append("\\").append(findData.cFileName), files, names);    //递归遍历目录下子目录
        }
        else{   //若为文件则将其加入列表中
            files.push_back(path.assign(dirPath).append("\\").append(findData.cFileName));  //文件路径加入列表
            names.push_back(findData.cFileName);    //文件名加入列表
        }
    }while(FindNextFile(hFile, &findData));
    FindClose(hFile);   //关闭句柄
}

/***
 * 在服务器创建目录（最好为绝对路径，比较稳妥）
 * @param sock
 * @param path 所需创建的路径
 * @return 成功创建开头前三位返回250，否则数字 > 300
 */
string mkd(SOCKET sock, string path ){  //在服务器创建目录
    char* p=(char*)malloc(clength);
    memset(p, 0, clength);
    sprintf(p, path.c_str());
    string r=SendCommand(sock, MKD, p);
    return r;
}