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
    }
    return count;
}

long downloadDir(SOCKET socket,string DirPath ,string storePath)
{
    int filecount=0;
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
                newport=SendCommand(socket,PASV);
                port=getNewPort(newport);
                dataSocket=getNewSocket("127.0.0.1",port);
                vector<File> subFileList=ls(socket,dataSocket,file.path);
                for(int i=0;i<subFileList.size();i++)
                {
                    fileQueue.push(files[i]);
                }
            } else{
                //普通文件,下载进入被动模式，调用即可
                stringstream storefP;
                storefP<<storePath;

                //TODO 此处具体还需要根据文件系统再定，看qt返回的文件路径的情况而定
                storefP<<"\\";
                storefP<<file.name;
                download(socket,file.path,storefP.str());
                filecount++;
            }
        }

    }
    return  filecount;
}
