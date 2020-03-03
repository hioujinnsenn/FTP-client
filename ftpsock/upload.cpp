//
// Created by asus on 2020/2/24.
//
#include "upload.h"

/***
 * 上传功能函数，上传本地文件到服务器指定路径位置
 * @param sock
 * @param uploadPath
 * @param filePath
 * @return 执行的结果以字符串形式返回并打印
 */
string uploadFile(SOCKET sock, string uploadPath, string filePath)     //上传文件到服务器
{
    SOCKET dataSocket=pasv(sock);   //开启被动模式，返回数据端口socket
    char* fileName=(char*)malloc(filePath.size()); //所需上传文件的文件名
    memset(fileName, 0,filePath.size());
    size_t offset=0;  //文件写偏移量，用于断点续传

    SendCommand(sock, "TYPE i\r\n");
    int i=(filePath.find_last_of('\\')!=string::npos) ? filePath.find_last_of('\\') : filePath.find_last_of('/');  //找到文件名前的分隔符
    sprintf(fileName, filePath.substr(i+1, string::npos).c_str());  //得到所需上传的文件名
    if(! uploadPath.empty()){    //若没有指定目录，则不需要change directory
        cwd(sock, uploadPath);   //改变工作目录
    }

    size_t sizeLocal=getFileSize(filePath);     //得到本地需要上传的文件的大小
    ifstream file(filePath, ios::binary);  //需要上传的文件流
    string size=SendCommand(sock,SIZE, fileName);   //用size命令判断要上传的文件是否已经在目录中
    if(size.substr(0, 3)=="213"){   //eg："213 7900000\r\n"否则505
        size_t sizeUp;
        sizeUp=stringToSize_t(size.substr(4,size.size()-4));
        if(sizeLocal<=sizeUp){  //上传文件不为残缺文件
            string r="文件已存在或重名！";
            cout<<r<<endl;
            free(fileName);
            file.close();
            return r;
        }
        else offset=sizeUp;
    }

    if(! file.is_open()){
        string r("文件打开失败！");
        cout<<r<<endl;
        return r;
    }
    file.seekg(offset, ios::beg);   //将file流的读取指针移到上传的文件内容后面，继续上传
    string mes=SendCommand(sock, APPE, fileName);  //请求上传文件，若不存在则新建，反之则在文件中接着上传
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
        size_t ptr=file.tellg();
        if(offset+Dlength>sizeLocal){
            size_t rlength=sizeLocal-offset;
            file.read(message, rlength+1);  //多读一个字节，判断已不能读，使得eof为true，从而避免死循环问题。否则永远无法多读一位，使eof为true
            send(dataSocket, message, rlength, 0);
            offset+=rlength;
        }
        else{
            file.read(message, Dlength);
            send(dataSocket, message, Dlength, 0);
            offset+=Dlength;
        }
        ptr=file.tellg();
        free(message);
    }
    file.close();   //清理现场
    free(fileName);
    string message=closeDataSock(sock, dataSocket);     //关闭数据端口
    cout<<message;
    return "文件上传成功";
}

string uploadDir(SOCKET sock, string uploadPath, string dirPath)    //上传文件夹到服务器指定目录下
{

}