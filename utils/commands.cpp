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
    cout<<message;
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