//
// Created by ye11 on 2020/2/29.
//

#include "download.h"
#define COMPLETE_CODE "226 Transfer complete"
long download(SOCKET socket,string Path,string storePath)
{   long count=0;
    string newport=SendCommand(socket,PASV);
    int port=getNewPort(newport);
    SOCKET dataSocket=getNewSocket("127.0.0.1",port);
    char*message=(char*)malloc(Dlength);
    memset(message,0,Dlength);
    sprintf(message,Path.data());
    long filesize=size(socket,Path);
    SendCommand(socket,"TYPE i\r\n");
    SendCommand(socket,"MODE S\r\n");
    string p=SendCommand(socket,RETR,message);
    string returnCode=p.substr(0,3);
    if(returnCode!="150")
    {
        return -1;
    }
    else{
        cout.flush();
        fstream file;
        file.open(storePath,ios::out|ios::binary);
        char* fileData=(char*)malloc(Dlength);
        memset(fileData,0,Dlength);
        while(count<filesize){
            long recvSize=recv(dataSocket,fileData,Dlength,0);
            cout<<"recvSize:"<<recvSize<<"\n";
            if(recvSize==0)
            {
                //中间传输遇到问题，需要断点续传\
                memset(message,0,Dlength);
                recv(socket,message,Dlength,0);
                string M=message;
                cout<<M;
                memset(message,0,Dlength);
                sprintf(message,Path.data());
                newport=SendCommand(socket,PASV);
                port=getNewPort(newport);
                dataSocket=getNewSocket("127.0.0.1",port);
                SendCommand(socket,"TYPE i\r\n");
                SendCommand(socket,REST,count);
                SendCommand(socket,RETR,message);
                recvSize=recv(dataSocket,fileData,Dlength,0);
            }
            file.write(fileData,recvSize);
            file.flush();
            count+=recvSize;
            memset(fileData,0,Dlength);
        }
        file.flush();
        file.close();
        recv(socket,message,Dlength,0);

    }
    free(message);
    return count;
}
/**
 * @details 整个文件夹的下载，拷贝文件夹结构
 * @fixed 已修复下载时的未能拷贝整个文件夹结构的问题
 * */
long downloadDir(SOCKET socket,string DirPath ,string storePath)
{
    int filecount=0;
    //创建本地文件夹
    mkdir_local(storePath);
    //进入被动模式，获取端口SOCKET
    string newport=SendCommand(socket,PASV);
    int port=getNewPort(newport);
    SOCKET dataSocket=getNewSocket("127.0.0.1",port);
    vector<File> files=ls(socket,dataSocket,DirPath);
    queue<File> fileQueue;
    //应该使用广度遍历优先队列
    for(int i=0;i<files.size();i++)
    {
        fileQueue.push(files[i]);
    }

    while(fileQueue.size()>0)
    {   int size=fileQueue.size();
        for(int i=0;i<size;i++)
        {  File file=fileQueue.front();
           fileQueue.pop();
            if(file.type==1)
            {
                //目录文件。
                //本地同步创建一个文件夹
                //不对   已修复此处的错误。
                string newDir=storePath;
                string DirTail=file.path.substr(DirPath.size());
                newDir.append(DirTail);
                if(!PathIsDirectory(newDir.data()))
                {
                    //实际上并不在乎是左斜杠还是右斜杠，因此不需要修改代码
                    CreateDirectory(newDir.data(),NULL);
                }
                newport=SendCommand(socket,PASV);
                port=getNewPort(newport);
                dataSocket=getNewSocket("127.0.0.1",port);
                vector<File> subFileList=ls(socket,dataSocket,file.path);
                for(int i=0;i<subFileList.size();i++)
                {
                    fileQueue.push(subFileList[i]);
                }
            } else{
                //普通文件,下载进入被动模式，调用即可
                //路径做了对应的修正，不只是简单的使用文件名，还应该复制整个文件夹的结构
                //还是不对路径还是错误的。

                string storefpath=storePath;
                string DirTail=file.path.substr(DirPath.size());
                storefpath.append(DirTail);
                download(socket,file.path,storefpath);
                filecount++;
            }
        }

    }
    return  filecount;
}
