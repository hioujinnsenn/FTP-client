//
// Created by ye11 on 2020/4/11.
//
#include "qThread.h"

// 下载断点续传，分为单个文件处理还有文件夹处理
bool qThread::downloadContinue() {
    if (this->currentMsg.isDir==1)
    {
        // storePath：与下载的目录对应的 本地保存的目录
        downloadDirContinue(this->currentMsg.filepath,this->currentMsg.storepath,this->currentMsg.finishNum,this->currentMsg.id);

    } else{
        long filesize=getFileSize(this->currentMsg.storepath);

        //  实际可以将这些参数省去，直接在函数内部掉用，只是，写出来是为了让自己明确知道使用到的关键信息
        //  storepath：文件的本地保存路径
        downloadFileContinue(this->currentMsg.filepath,this->currentMsg.storepath,filesize);
    }
}

//  被run调用的下载
bool qThread::download(string filePath, int id) {
    if(isDirRemote(filePath))
    {
        this->currentMsg.isDir=1;  //1是目录，0是文件
        downloadDir(filePath,id);
    } else{
        this->currentMsg.isDir=0;  //必须声明
        downloadFile(filePath,id);
    }
}


// 断点续传，单个文件的处理
bool qThread::downloadFileContinue( string filePath, string storePath, long downloadsize) {
    SOCKET sock=login(this->Username,this->Password,this->Ip);
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
        if(this->currentMsg.status==1)
        {
            SendCommand(sock,"ABOR\r\n");
            SendCommand(sock,QUIT);
            break;                               //跳出结束此次任务，同时，不能让finishedOne信号发出
        }
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

        // 更新进度
        downloadsize+=recvSize;
        emit sendProgress(int(100*downloadsize*1.0/sizeTotal),id);
        memset(fileData,0,Dlength);
    }
    free(fileData);
    free(filechars);
    file.flush();
    file.close();
    SendCommand(sock,QUIT);
}

// 自定义路径的下载文件，主要用于dir中的文件下载
// 增加断点续传支持
// 实际上参数可以不这么传递，毕竟都写在this->currentMsg里了
// 但是只是为了让自己注意需要管理的数据主要那些
/**
 *  @details 通过目录的文件下载，增加断点续传支持
 *  TODO 可以更改设计成：每个文件的下载都是独立的一次SOCK过程
 *  @param filecount: 当前元素的编号，也等于已经完成的文件数
 *  @return   1：下载正常结束 2：被暂停  -1：异常结束
 * */
int qThread::downloadDirFile( string filePath, string storePath,int filecount) {
    SOCKET  sock=login(this->Username,this->Password,this->Ip);
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
            if(this->currentMsg.status==1)  //项目暂停
            {   // 保存当前元素的编号

                SendCommand(sock,"ABOR\r\n");     //终止数据传输
                SendCommand(sock,QUIT);
                return 2;
            }
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
            memset(fileData,0,Dlength);
        }
        file.flush();
        file.close();
        recv(sock,message,Dlength,0);  // 接受处理完的返回信息
        free(fileData);
    }
    free(message);
    SendCommand(sock,QUIT);
    return 1;         //执行正确的话，返回1
}

// 默认无路径的下载文件函数，嵌入到数据线程中
bool  qThread::downloadFile(string Path,int  id)
{
    /**
     *  socket：命令端口
     *  Path：远程端的路径，不指定本地路径，默认在对应当前目录
     *  id：下载的唯一id识别值
     * */

    SOCKET  socket=login(this->Username,this->Password,this->Ip);
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

        this->currentMsg.storepath=storePath;           //保存路径到this->currentMg,方便断点续传恢复
        file.open(QIODevice::WriteOnly);                //此处不涉及断点续传，直接覆盖
        char* fileData=(char*)malloc(Dlength);
        memset(fileData,0,Dlength);
        while(count<filesize){
            // 触发暂停后的响应
            if(this->currentMsg.status==1)
            {
                // 只需要直接结束下载，中断服务器的服务
                SendCommand(socket,"ABOR\r\n");
                SendCommand(socket,QUIT);
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
    SendCommand(socket,QUIT);
    return count;
}



// 重写的目录下载
// 增加了文件夹断点续传支持 2020.4.13
bool qThread::downloadDir(string filePath,int id){
    SOCKET  sock=login(this->Username,this->Password,this->Ip);
    // 切换回当前目录
    char* cwdPath=(char*)malloc(400);
    memset(cwdPath,0,400);
    sprintf(cwdPath,this->remote_path.data());
    SendCommand(sock,CWD,cwdPath);
    free(cwdPath);
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
    //
    //应该使用广度遍历优先队列
    //
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
    // 一次循环获取所有的需要下载文件数，同时创建子文件夹
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
                for(int j=0;j<subFileList.size();j++)
                {
                    fileQueue.push(subFileList[j]);
                    if(subFileList[j].type==2)  //是文件类型才保存到下载队列
                    {
                        fileDownloadQueue.push(subFileList[j]);
                        filesCount++;
                    }
                }
            }
        }
    }
    SendCommand(sock,QUIT);  //作为获取文件列表而使用的sock，可以结束了，此后的sock下载的自己开
    //fileQueue是为了正确的层次遍历而使用的
    //fileDownloadQueue是为了下载使用的，存放所有文件的完整路径和信息

    int filecount=0;   //已完成的文件数目
    while(fileDownloadQueue.size()>0)
    {

        File file=fileDownloadQueue.front();
        fileDownloadQueue.pop();
        // 文件夹内部，重新建立一个sock，旧的sock在此处QUIT
        sock=login(this->Username,this->Password,this->Ip);   // 重新登录
        //普通文件,下载进入被动模式，调用即可
        //路径做了对应的修正，不只是简单的使用文件名，还应该复制整个文件夹的结构
        //还是不对路径还是错误的。
        string storefpath=storePath;                          // 存储的父路径
        string DirTail=file.path.substr(filePath.size());
        storefpath.append(DirTail);

        //  IMPORTANT  使用的是调用目录处理的专属函数，跟一般用的不太一样
        int pauseOrNot= downloadDirFile(file.path,storefpath,filecount);
        if(pauseOrNot==2)
        {
            // 文件下载项目暂停
            return  filecount;     //直接结束当前项目
        }
        filecount++;
        this->currentMsg.finishNum=filecount;                 //记录已经完成的数目
        emit sendProgress(int(100*(filecount*1.0/filesCount)),id);
    }
    return  filecount;
}


// 断点续传：目录的，处理方式更普通文件不同
// 记录当前正在下载的文件，文件顺序是固定的，只需要获取文件列表逐个遍历即可
bool qThread::downloadDirContinue(string DirPath,string storePath, int finishedNum, int id) {

    // 如后期不稳定，应改成每个单独文件创建一个sock命令端口
    // 创建本地文件夹
    SOCKET  sock=login(this->Username,this->Password,this->Ip);
    char* cwdPath=(char*)malloc(400);
    memset(cwdPath,0,400);
    sprintf(cwdPath,this->remote_path.data());
    SendCommand(sock,CWD,cwdPath);
    free(cwdPath);
    mkdir_local(storePath);
    //进入被动模式，获取端口SOCKET
    string newport=SendCommand(sock,PASV);
    int port=getNewPort(newport);
    SOCKET dataSocket=getNewSocket("127.0.0.1",port);
    vector<File> files=ls(sock,dataSocket,DirPath);
    queue<File> fileQueue;
    queue<File> fileDownloadQueue;
    int filesCount=0;
    //
    //应该使用广度遍历优先队列
    //
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
    // 一次循环获取所有的需要下载文件数，同时创建子文件夹
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
                string DirTail=file.path.substr(DirPath.size());       //对比截取掉前面的文件路径，保存后面的目录内细节部分
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
                for(int j=0;j<subFileList.size();j++)
                {
                    fileQueue.push(subFileList[j]);
                    if(subFileList[j].type==2)  //是文件类型才保存到下载队列
                    {
                        fileDownloadQueue.push(subFileList[j]);
                        filesCount++;
                    }
                }
            }
        }
    }

    // 作为获取文件列表的sock，任务已完成，为了程序稳定性，直接关掉，每个下载任务自己重开
    SendCommand(sock,QUIT);

    //fileQueue是为了正确的层次遍历而使用的
    //fileDownloadQueue是为了下载使用的，存放所有文件的完整路径和信息
    for(int i=0;fileDownloadQueue.size()>0&&i<finishedNum;i++)
    {
        fileDownloadQueue.pop();
    }
    int HaveOnePause=1;           //有一个被中断的元素，他的处理不一样
    while (fileDownloadQueue.size()>0)
    {
        File file=fileDownloadQueue.front();
        fileDownloadQueue.pop();
        string storefpath=storePath;                          // 存储的父路径
        string DirTail=file.path.substr(DirPath.size());
        storefpath.append(DirTail);

        if(HaveOnePause==1)       //只有一个需要断点续传，其他正常传
        {
            long loadedSize=getFileSize(storefpath);
            int PauseOrNot=downloadDirStopedFileContinue(file.path,storefpath,finishedNum,loadedSize);
            if(PauseOrNot==2)
            {
                return true;
            }
            HaveOnePause=0;
        }
        else{
            int PauseOrNot=downloadDirFile(file.path,storefpath,finishedNum);
            if(PauseOrNot==2)
            {
                return true;
            }
        }
        finishedNum++;
        emit sendProgress(int(100* finishedNum*1.0/filesCount),id);  //更新进度
        this->currentMsg.finishNum=finishedNum;      //必须实时更新
    }
}

/**
 * @details：文件夹内文件的断点续传，只有一个需要调用此函数，其他的都是正常处理
 *           由于逻辑有细微区别，避免某个函数内部逻辑过于复杂，从而拆分开
 * */
int qThread::downloadDirStopedFileContinue(string filePath, string storePath,  int filecount,long downloadsize) {
    SOCKET  sock=login(this->Username,this->Password,this->Ip);
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
        if(this->currentMsg.status==1)
        {
            // 对队列的操作统一放在槽函数里做
            SendCommand(sock,"ABOR\r\n");        //结束数据传输
            SendCommand(sock,QUIT);
            return 2;                               //跳出结束此次任务，同时，不能让finishedOne信号发出
        }
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
        memset(fileData,0,Dlength);
    }
    free(fileData);
    free(filechars);
    file.flush();
    file.close();
    SendCommand(sock,QUIT);
    return 1;

}

bool qThread::isDirRemote(string filePath)
{
    SOCKET sock=login(this->Username, this->Password, this->Ip);
    string r=cwd(sock, filePath);   //改变服务器工作目录
    //该目录不存在(返回>300的数)，说明该路径为文件
        SendCommand(sock, QUIT);
        return !(r.substr(0, 3) > "300");
}