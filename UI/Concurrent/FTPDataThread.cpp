//
// Created by ye11 on 2020/3/20.
//

#include "FTPDataThread.h"
FTPDataThread::FTPDataThread(QObject* parent):
 QThread(parent)
{

}
//  重点：emit 触发信号，引发信号/槽机制
void FTPDataThread::run() {

    for(int i=0;i<fileList.size();i++)
    {
          long TotalSize=size(commandSocket,fileList[i].path);
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

    }
}
void FTPDataThread::setupPath(string path) {
    storePath=path;
}
void FTPDataThread::setupList(vector<File> list){
    for(int i=0;i<list.size();i++)
    {
        fileList.push_back(list[i]);
    }
}