//
// Created by ye11 on 2020/4/11.
//
#include "upload_qThread.h"

// 下载断点续传，分为单个文件处理还有文件夹处理
bool uploadThread::downloadContinue(SOCKET sock) {
    if (this->currentMsg.isDir==1)
    {

    } else{
        cout<<"文件断点续传"<<endl;
        downloadFileContinue(sock,this->currentMsg.filepath,this->currentMsg.storepath,this->currentMsg.id,this->currentMsg.filesize);
    }
}

//  被run调用的下载
bool uploadThread::download(SOCKET sock, string filePath, int id) {
    if(PathIsDirectory(filePath.data()))
    {
        downloadDir(sock,filePath,id);
    } else{
        downloadFile(sock,filePath,id);
    }
}


// 断点续传，单个文件的处理
bool uploadThread::downloadFileContinue(SOCKET sock, string filePath, string storePath, int id, long downloadsize) {
    SOCKET dataSock=pasv(sock);                  // 被动模式
    SendCommand(sock,"TYPE i\r\n");           // 二进制数据传输
    SendCommand(sock,"MODE S\r\n");           // mode s代表什么传输流模式
    SendCommand(sock,REST,downloadsize);         // 跳过
    char * filechars=(char*)malloc(400);
    memset(filechars,0,400);
    sprintf(filechars,filePath.data());
    long sizeTotal=size(sock,filePath);
    QFile file(storePath.data());
    file.open(QIODevice::Append);            //此处不涉及断点续传，直接覆盖
    file.skip(downloadsize);                 //跳过前面的多少字节
    char* fileData=(char*)malloc(Dlength);
    memset(fileData,0,Dlength);
    SendCommand(sock,RETR,filechars);
    while(downloadsize<sizeTotal)
    {
        long recvSize=recv(dataSock,fileData,Dlength,0);
        if(recvSize==0)       //如果发生中断，则设置暂停和处理断点续传
        {                     //文件夹的中断处理，需要进行标记

            dataSock=pasv(sock);
            SendCommand(sock,"TYPE i\r\n");
            SendCommand(sock,REST,downloadsize);
            SendCommand(sock,RETR,filechars);
            recvSize=recv(dataSock,fileData,Dlength,0);
        }
        file.write(fileData,recvSize);
        file.flush();
        downloadsize+=recvSize;
        if (id!=-1)  //  -1 代表通过下载目录而下载的文件
                emit sendProgress(int(100*(downloadsize*1.0/sizeTotal)),id);    //更新进度条
        memset(fileData,0,Dlength);
    }
    free(fileData);
    free(filechars);
    file.flush();
    file.close();
}

// 自定义路径的下载文件，主要用于dir中的文件下载
bool uploadThread::downloadFile(SOCKET sock, string filePath, string storePath, int id) {
    long count=0;
    string newport=SendCommand(sock,PASV);
    int port=getNewPort(newport);
    SOCKET dataSocket=getNewSocket("127.0.0.1",port);
    char*message=(char*)malloc(Dlength);
    memset(message,0,Dlength);
    sprintf(message,filePath.data());
    long filesize=size(sock,filePath);
    SendCommand(sock,"TYPE i\r\n");            // 二进制数据传输
    SendCommand(sock,"MODE S\r\n");           // mode s代表什么传输流模式
    string p=SendCommand(sock,RETR,message);    //  RETR 下载模式
    string returnCode=p.substr(0,3);
    if(returnCode!="150")
    {
        return -1;
    }
    else{
        cout.flush();
        QFile file(storePath.data());
        file.open(QIODevice::WriteOnly);           //此处不涉及断点续传，直接覆盖
        char* fileData=(char*)malloc(Dlength);
        memset(fileData,0,Dlength);
        while(count<filesize){
            long recvSize=recv(dataSocket,fileData,Dlength,0);
            if(recvSize==0)       //如果发生中断，则设置暂停和处理断点续传
            {                     //文件夹的中断处理，需要进行标记
                memset(message,0,Dlength);
                sprintf(message,filePath.data());
                newport=SendCommand(sock,PASV);
                port=getNewPort(newport);
                dataSocket=getNewSocket("127.0.0.1",port);
                SendCommand(sock,"TYPE i\r\n");
                SendCommand(sock,REST,count);
                SendCommand(sock,RETR,message);
                recvSize=recv(dataSocket,fileData,Dlength,0);
            }
            file.write(fileData,recvSize);
            file.flush();
            count+=recvSize;
            if (id!=-1)  //  -1 代表通过下载目录而下载的文件
                    emit sendProgress(int(100*(count*1.0/filesize)),id);    //更新进度条
            memset(fileData,0,Dlength);
        }
        file.flush();
        file.close();
        recv(sock,message,Dlength,0);  // 接受处理完的返回信息
        free(fileData);
    }
    free(message);

    return count;
}

// 默认无路径的下载文件函数，嵌入到数据线程中
bool  uploadThread::downloadFile(SOCKET socket,string Path,int  id)
{
    /**
     *  socket：命令端口
     *  Path：远程端的路径，不指定本地路径，默认在对应当前目录
     *  id：下载的唯一id识别值
     * */
    long count=0;
    string newport=SendCommand(socket,PASV);
    int port=getNewPort(newport);
    SOCKET dataSocket=getNewSocket("127.0.0.1",port);
    char*message=(char*)malloc(Dlength);
    memset(message,0,Dlength);
    sprintf(message,Path.data());
    long filesize=size(socket,Path);
    SendCommand(socket,"TYPE i\r\n");            // 二进制数据传输
    SendCommand(socket,"MODE S\r\n");           // mode s代表什么传输流模式
    string p=SendCommand(socket,RETR,message);    //  RETR 下载模式
    string returnCode=p.substr(0,3);
    if(returnCode!="150")
    {
        return -1;
    }
    else{
        cout.flush();
        string storePath=this->local_path;
        int pos=Path.find_last_of("/");
        if( pos==-1)
            pos=Path.find_last_of("\\");
        string filename=Path.substr(pos);
        storePath.append(filename);                     // 拼接本地路径
        QFile file(storePath.data());
        file.open(QIODevice::WriteOnly);           //此处不涉及断点续传，直接覆盖
        char* fileData=(char*)malloc(Dlength);
        memset(fileData,0,Dlength);
        while(count<filesize){
            // 触发暂停后的响应
            if(this->currentMsg.status==1)
            {
                // 暂停当前项目,项目回到队列尾，记录此刻的状态
                // 当前下载的是文件，只需要保存一个文件需要的信息即可
                this->currentMsg.filesize=count;   //记录已经下载的文件大小
                this->currentMsg.UpOrDown=3;       //切换成下载的断点续传任务
                this->currentMsg.isDir=0;         //0为文件
                this->currentMsg.storepath=storePath;   //记录存储路径
                this->msgs.push_back(this->currentMsg);     //返回队尾
                break;                               //跳出结束此次任务，同时，不能让finishedOne信号发出
            }
            long recvSize=recv(dataSocket,fileData,Dlength,0);
            if(recvSize==0)       //如果发生中断，则进行断点处理
            {
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
            if (id!=-1)  //  -1 代表通过下载目录而下载的文件
                    emit sendProgress(int(100*(count*1.0/filesize)),id);    //更新进度条
            memset(fileData,0,Dlength);
        }
        file.flush();
        file.close();
    }
    free(message);
    return count;
}



// 重写的目录下载
bool uploadThread::downloadDir(SOCKET sock,string filePath,int id){
    //创建本地文件夹
    string storePath=this->local_path;
    int pos=filePath.find_last_of("/");
    if(pos==-1)
        pos=filePath.find_last_of("\\");
    storePath.append(filePath.substr(pos));      //拼接本地路径
    mkdir_local(storePath);
    //进入被动模式，获取端口SOCKET
    string newport=SendCommand(sock,PASV);
    int port=getNewPort(newport);
    SOCKET dataSocket=getNewSocket("127.0.0.1",port);
    vector<File> files=ls(sock,dataSocket,filePath);
    queue<File> fileQueue;
    queue<File> fileDownloadQueue;
    int filesCount=0;
    //应该使用广度遍历优先队列
    for(int i=0;i<files.size();i++)
    {
        fileQueue.push(files[i]);
        if(files[i].type==2)  //是文件类型才保存到下载队列
        {
            fileDownloadQueue.push(files[i]);
            filesCount++;
        }
    }
    // 计算出总共的需要下载的文件数后，要回传修改对应的文件夹大小
    emit  send_Dir_filecount(filesCount,id);
    // 一次循环获取所有的需要下载文件数，同时子创建文件夹
    while(fileQueue.size()>0)
    {   int size=fileQueue.size();
        for(int i=0;i<size;i++)
        {   File file=fileQueue.front();
            fileQueue.pop();
            if(file.type==1)
            {
                //目录文件。
                //本地同步创建一个文件夹
                string newDir=storePath;
                string DirTail=file.path.substr(filePath.size());       //对比截取掉前面的文件路径，保存后面的目录内细节部分
                newDir.append(DirTail);
                if(!PathIsDirectory(newDir.data()))
                {
                    //实际上并不在乎是左斜杠还是右斜杠，因此不需要修改代码
                    CreateDirectory(newDir.data(),NULL);
                }
                newport=SendCommand(sock,PASV);
                port=getNewPort(newport);
                dataSocket=getNewSocket("127.0.0.1",port);
                vector<File> subFileList=ls(sock,dataSocket,file.path);
                for(int j=0;i<subFileList.size();j++)
                {
                    fileQueue.push(subFileList[j]);
                    if(files[i].type==2)  //是文件类型才保存到下载队列
                    {
                        fileDownloadQueue.push(subFileList[j]);
                        filesCount++;
                    }
                }
            }
        }
    }
    int filecount=0;
    while(fileDownloadQueue.size()>0)
    {

        File file=fileDownloadQueue.front();
        fileDownloadQueue.pop();
        sock=login(this->Username,this->Password,this->Ip);   // 重新登录
        //普通文件,下载进入被动模式，调用即可
        //路径做了对应的修正，不只是简单的使用文件名，还应该复制整个文件夹的结构
        //还是不对路径还是错误的。
        string storefpath=storePath;          // 存储的父路径
        string DirTail=file.path.substr(filePath.size());
        storefpath.append(DirTail);
        downloadFile(sock,file.path,storefpath,-1);
        filecount++;
        emit sendProgress(int(100*(filecount*1.0/filesCount)),id);
    }
    return  filecount;
}

