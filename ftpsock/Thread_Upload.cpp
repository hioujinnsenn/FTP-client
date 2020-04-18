//
// Created by ye11 on 2020/4/11.
//
#include "qThread.h"
#define Slength 500
// 拆分upload_qThread的代码

/***
 * 上传的主函数
 * @param sock 控制端口
 * @param uploadPath 需要上传到的目录路径（必须是绝对路径）
 * @param filePath 所需上传的文件所在本地路径
 * @return 返回参数可修改，目前为string
 */
bool qThread::upload(string filePath, int id)
{
    bool result;
    if(PathIsDirectory(filePath.data())){   //是目录路径
        this->currentMsg.isDir=1;
        result=uploadDir(filePath,id);
    }
    else{   //是文件路径
        this->currentMsg.isDir=0;
        result=uploadFile(filePath, id);
    }
    return result;
}

/***
 * 上传功能函数，上传本地文件到服务器指定路径位置
 * @param sock
 * @param uploadPath
 * @param filePath
 * @return 执行的结果以字符串形式返回并打印
 * offset参数可作为上传进度的参考值，与sizeLocal的比值即可判断上传进度
 */
bool qThread::uploadFile(string filePath, int id)     //上传文件到服务器
{
    SOCKET sock=login(this->Username, this->Password, this->Ip);
    SendCommand(sock, "TYPE i\r\n");
    SOCKET dataSock=pasv(sock);   //开启被动模式，返回数据端口socket
    string storePath=this->remote_path;
    cwd(sock, storePath);

    this->currentMsg.storepath=storePath;
    char* fileName=(char*)malloc(filePath.size()); //所需上传文件的文件名
    memset(fileName, 0,filePath.size());
    int i=(filePath.find_last_of('\\')!=string::npos) ? filePath.find_last_of('\\') : filePath.find_last_of('/');  //找到文件名前的分隔符
    sprintf(fileName, filePath.substr(i+1, string::npos).c_str());  //得到所需上传的文件名
    size_t sizeLocal=getFileSize(filePath);     //得到本地需要上传的文件的大小
    QFile file(QString::fromStdString(filePath));
    if(! file.open(QIODevice::ReadOnly)){
        string r("文件打开失败！");
        cout<<filePath<<r<<endl;
        SendCommand(sock,"ABOR\r\n");
        closesocket(dataSock);
        SendCommand(sock,QUIT);
        return false;
    }
    string mes=SendCommand(sock, STOR, fileName);  //请求上传文件，若不存在则新建，反之则在文件中接着上传
    string m=mes.substr(0,2);
    if(m>"300" || m.empty()){
        cout<<"文件上传失败"<<endl;
        file.close();
        free(fileName);
        SendCommand(sock,"ABOR\r\n");     //终止数据传输
        closesocket(dataSock);
        SendCommand(sock,QUIT);
        return false;   //失败返回
    }
    long upSize=filesize(sock,fileName);
    int offset=0;

    while (!file.atEnd()) {    //在到达文件末尾前持续读文件，将文件内容通过数据端口上传到服务器
        if (this->currentMsg.status ) {   //被暂停或终止的项目
            SendCommand(sock,"ABOR\r\n");
            SendCommand(sock,QUIT);
            return false;
        }
        char *message = (char *) malloc(Slength);   //dataBuffer
        memset(message, 0, Slength);
        size_t rlength = file.read(message, Slength); //read返回当前读到的字节数
        send(dataSock, message, rlength, 0);
        offset+=rlength;
        free(message);
        emit sendProgress(int(100 * (offset / (sizeLocal*1.0) )), id);    //将文件发送进度
    }
    while(upSize<sizeLocal)                    //使用两层循环，一层上传，一层检测数据
    {
        if (this->currentMsg.status ) {   //被暂停或终止的项目
            free(fileName);
            SendCommand(sock,"ABOR\r\n");
            SendCommand(sock,QUIT);
            cout << "项目被暂停！" << endl;
            return false;
        }
        msleep(300);
        upSize=filesize(sock,fileName);
    }
    file.close();   //清理现场
    free(fileName);
    string s = closeDataSock(sock, dataSock);     //关闭数据端口
    cout << s;
    flush(cout);
    SendCommand(sock, QUIT);
    return true;
}

/***
 * 上传文件夹的函数。由upload函数内部调用
 * @param sock 控制端口
 * @param uploadPath 传入的上传到的目录位置（空则为当前工作目录下）
 * @param dirPath 所需上传的文件夹的本地路径
 * @return
 * 下面的i和files.size()比值可得当前文件夹上传的进度（以文件夹中上传的文件数为计量单位）
 */
bool qThread::uploadDir(string dirPath, int id)    //上传文件夹到服务器指定目录下
{
    SOCKET sock=login(this->Username, this->Password, this->Ip);
    string uploadPath=this->remote_path;  //得到需要上传到的目录
    cwd(sock, uploadPath);

    this->currentMsg.storepath=uploadPath;
    vector<string> files, names;
    string dirName=dirPath.substr(dirPath.find_last_of("\\")+1, dirPath.size());
    getAllFiles(dirPath, files, names);       //得到目录下所有的文件路径和名称，两者顺序对应
    if(files.size()!=names.size()){
        cout<<"查找目录子文件出错！路径数和文件名数不等"<<endl;
        return false;
    }
    emit send_Dir_filecount(files.size(),id);        // 通知UI线程文件总数
    SendCommand(sock,QUIT);
    for(int i=0; i<files.size(); i++){
        this->currentMsg.finishNum=i;
        string path(files.at(i));   //当前文件路径
        string name(names.at(i));   //当前文件名
        int pos=dirPath.find_last_of("/");
        if(pos==-1) pos=dirPath.find_last_of("\\");
        string wd;
        if(uploadPath=="\\" || uploadPath=="/")
            wd=uploadPath+path.substr(pos+1, path.size()-name.size()-pos-2);   //得到文件所在路径
        else  wd=uploadPath+"/"+path.substr(pos+1, path.size()-name.size()-pos-2);
        if(! uploadDirFile(path,id, wd))
            return false;
        emit sendProgress(100*(i+1)/files.size(), id);    //上传目录的进度
    }
    return true;
}

bool qThread::uploadDirFile(string filePath, int id, string uploadPath)     //上传文件到服务器
{
    SOCKET sock=login(this->Username, this->Password, this->Ip);
    SendCommand(sock, "TYPE i\r\n");
    SOCKET dataSock=pasv(sock);   //开启被动模式，返回数据端口socket
    string r=cwd(sock, uploadPath);   //改变服务器工作目录
    if(r.substr(0,3)>"300"){    //该目录不存在
        mkd(sock, uploadPath);  //创建目录
        cwd(sock, uploadPath);  //进入目录
    }

    char* fileName=(char*)malloc(filePath.size()); //所需上传文件的文件名
    memset(fileName, 0,filePath.size());
    int i=(filePath.find_last_of('\\')!=string::npos) ? filePath.find_last_of('\\') : filePath.find_last_of('/');  //找到文件名前的分隔符
    sprintf(fileName, filePath.substr(i+1, string::npos).c_str());  //得到所需上传的文件名
    size_t sizeLocal=getFileSize(filePath);     //得到本地需要上传的文件的大小
    QFile file(QString::fromStdString(filePath));
    if(! file.open(QIODevice::ReadOnly)){
        string r("文件打开失败！");
        cout<<filePath<<r<<endl;
        SendCommand(sock,"ABOR\r\n");
        closesocket(dataSock);
        SendCommand(sock,QUIT);
        return false;
    }
    string mes=SendCommand(sock, STOR, fileName);  //请求上传文件，若不存在则新建，反之则在文件中接着上传
    string m=mes.substr(0,2);
    if(m>"300" || m.empty()){
        cout<<"文件上传失败"<<endl;
        file.close();
        free(fileName);
        SendCommand(sock,"ABOR\r\n");     //终止数据传输
        closesocket(dataSock);
        SendCommand(sock,QUIT);
        return false;   //失败返回
    }
    while (!file.atEnd()) {    //在到达文件末尾前持续读文件，将文件内容通过数据端口上传到服务器
        if (this->currentMsg.status ) {   //被暂停或终止的项目
            SendCommand(sock,"ABOR\r\n");
            SendCommand(sock,QUIT);
            cout << "项目被暂停！" << endl;
            return false;
        }
        char *message = (char *) malloc(Slength);   //dataBuffer
        memset(message, 0, Slength);
        size_t rlength = file.read(message, Slength); //read返回当前读到的字节数
        send(dataSock, message, rlength, 0);
        free(message);
    }
    long upSize=filesize(sock,fileName);
    while(upSize<sizeLocal)                    //使用两层循环，一层上传，一层检测数据
    {
        if (this->currentMsg.status ) {   //被暂停或终止的项目
            free(fileName);
            SendCommand(sock,"ABOR\r\n");
            SendCommand(sock,QUIT);
            cout << "项目被暂停！" << endl;
            return false;
        }
        msleep(300);
        upSize=filesize(sock,fileName);
    }
    file.close();   //清理现场
    free(fileName);
    string s = closeDataSock(sock, dataSock);     //关闭数据端口
    SendCommand(sock, QUIT);
    return true;
}

long qThread::filesize(SOCKET sock, char* filename){
    char*  path=(char*)malloc(Dlength);
    memset(path,0,Dlength);
    sprintf(path,filename);
    string p=SendCommand(sock,SIZE,path);
    if(p.substr(0,3)>"330"){    //文件不存在
        return 0;
    }
    int i=p.find_last_of(' ');
    string size=p.substr(i+1,(p.size()-i-3));  //p.size()-2 -(i+1)
    istringstream  is(size);
    long sizel;
    is>>sizel;
    free(path);
    return sizel;
}

bool qThread::uploadContinue()
{
    if(this->currentMsg.isDir){ //文件夹断点续传
        uploadDirContinue(this->currentMsg.filepath, this->currentMsg.storepath, this->currentMsg.finishNum);
    }
    else{   //文件断点续传
        uploadFileContinue(this->currentMsg.filepath, this->currentMsg.storepath);
    }
}

bool qThread::uploadFileContinue(string filePath, string storePath)
{
    SOCKET sock=login(this->Username, this->Password, this->Ip);
    SendCommand(sock, "TYPE i\r\n");
    SOCKET dataSock=pasv(sock);   //开启被动模式，返回数据端口socket
    cwd(sock, storePath);
    char* fileName=(char*)malloc(filePath.size()); //所需上传文件的文件名
    memset(fileName, 0,filePath.size());
    int i=(filePath.find_last_of('\\')!=string::npos) ? filePath.find_last_of('\\') : filePath.find_last_of('/');  //找到文件名前的分隔符
    sprintf(fileName, filePath.substr(i+1, string::npos).c_str());  //得到所需上传的文件名
    size_t sizeLocal=getFileSize(filePath);     //得到本地需要上传的文件的大小
    size_t size=filesize(sock, fileName);   //得到当前文件在服务器的大小

    QFile file(QString::fromStdString(filePath));
    if(! file.open(QIODevice::ReadOnly)){
        string r("文件打开失败！");
        cout<<filePath<<r<<endl;
        SendCommand(sock,"ABOR\r\n");
        closesocket(dataSock);
        SendCommand(sock,QUIT);
        return false;
    }
    file.skip(size);
    SendCommand(sock, APPE, fileName);
    int offset=size;
    if( ! this->currentMsg.isDir)
        emit sendProgress(int(100 * (offset / (sizeLocal*1.0) )), id);  //初始化断点续传开始的进度
    while(!file.atEnd()){
        if (this->currentMsg.status ) {   //被暂停或终止的项目
            SendCommand(sock,"ABOR\r\n");
            SendCommand(sock,QUIT);
            return false;
        }
        char *message = (char *) malloc(Slength);   //dataBuffer
        memset(message, 0, Slength);
        size_t rlength = file.read(message, Slength); //read返回当前读到的字节数
        send(dataSock, message, rlength, 0);
        offset+=rlength;
        free(message);
        if(! this->currentMsg.isDir)
            emit sendProgress(int(100 * (offset / (sizeLocal*1.0) )), id);    //将文件发送进度
    }
    long upSize=filesize(sock,fileName);
    while(upSize<sizeLocal){    //确保文件上传完，而不在缓冲区
        if (this->currentMsg.status ) {   //被暂停或终止的项目
            free(fileName);
            SendCommand(sock,"ABOR\r\n");
            SendCommand(sock,QUIT);
            cout << "项目被暂停！" << endl;
            return false;
        }
        msleep(300);
        upSize=filesize(sock, fileName);
    }
    file.close();   //清理现场
    free(fileName);
    string s = closeDataSock(sock, dataSock);     //关闭数据端口
    cout << s;
    flush(cout);
    SendCommand(sock, QUIT);
    return true;
}

bool qThread::uploadDirContinue(string dirPath, string storePath, int filecount)
{
    SOCKET sock=login(this->Username, this->Password, this->Ip);
    cwd(sock, storePath);
    vector<string> files, names;
    string dirName=dirPath.substr(dirPath.find_last_of("\\")+1, dirPath.size());
    getAllFiles(dirPath, files, names);       //得到目录下所有的文件路径和名称，两者顺序对应
    if(files.size()!=names.size()){
        cout<<"查找目录子文件出错！路径数和文件名数不等"<<endl;
        return false;
    }
    emit send_Dir_filecount(files.size(),id);        // 通知UI线程文件总数
    emit sendProgress(100*(filecount+1)/files.size(), id);
    SendCommand(sock,QUIT);
    for(int i=filecount; i<files.size(); i++){
        this->currentMsg.finishNum=i;
        string path(files.at(i));   //当前文件路径
        string name(names.at(i));   //当前文件名
        int pos=dirPath.find_last_of("/");
        if(pos==-1) pos=dirPath.find_last_of("\\");
        string wd;
        if(storePath=="\\" || storePath=="/")
            wd=storePath+path.substr(pos+1, path.size()-name.size()-pos-2);   //得到文件所在路径
        else  wd=storePath+"/"+path.substr(pos+1, path.size()-name.size()-pos-2);
        if(! uploadDirFileContinue(path, wd))
            return false;
        emit sendProgress(100*(i+1)/files.size(), id);    //上传目录的进度
    }
    return true;
}

bool qThread::uploadDirFileContinue(string filePath, string uploadPath)     //上传文件到服务器
{
    SOCKET sock=login(this->Username, this->Password, this->Ip);
    SendCommand(sock, "TYPE i\r\n");
    SOCKET dataSock=pasv(sock);   //开启被动模式，返回数据端口socket
    string r=cwd(sock, uploadPath);   //改变服务器工作目录
    if(r.substr(0,3)>"300"){    //该目录不存在
        mkd(sock, uploadPath);  //创建目录
        cwd(sock, uploadPath);  //进入目录
    }

    char* fileName=(char*)malloc(filePath.size()); //所需上传文件的文件名
    memset(fileName, 0,filePath.size());
    int i=(filePath.find_last_of('\\')!=string::npos) ? filePath.find_last_of('\\') : filePath.find_last_of('/');  //找到文件名前的分隔符
    sprintf(fileName, filePath.substr(i+1, string::npos).c_str());  //得到所需上传的文件名
    size_t sizeLocal=getFileSize(filePath);     //得到本地需要上传的文件的大小
    size_t size=filesize(sock, fileName);   //得到当前文件在服务器的大小

    QFile file(QString::fromStdString(filePath));
    if(! file.open(QIODevice::ReadOnly)){
        string r("文件打开失败！");
        cout<<filePath<<r<<endl;
        SendCommand(sock,"ABOR\r\n");
        closesocket(dataSock);
        SendCommand(sock,QUIT);
        return false;
    }
    file.skip(size);
    string mes=SendCommand(sock, APPE, fileName);  //请求上传文件，若不存在则新建，反之则在文件中接着上传
    string m=mes.substr(0,2);
    if(m>"300" || m.empty()){
        cout<<"文件上传失败"<<endl;
        file.close();
        free(fileName);
        SendCommand(sock,"ABOR\r\n");     //终止数据传输
        closesocket(dataSock);
        SendCommand(sock,QUIT);
        return false;   //失败返回
    }
    while (!file.atEnd()) {    //在到达文件末尾前持续读文件，将文件内容通过数据端口上传到服务器
        if (this->currentMsg.status ) {   //被暂停或终止的项目
            free(fileName);
            SendCommand(sock,"ABOR\r\n");
            SendCommand(sock,QUIT);
            cout << "项目被暂停！" << endl;
            return false;
        }
        char *message = (char *) malloc(Slength);   //dataBuffer
        memset(message, 0, Slength);
        size_t rlength = file.read(message, Slength); //read返回当前读到的字节数
        send(dataSock, message, rlength, 0);
        free(message);
    }
    long upSize=filesize(sock,fileName);
    while(upSize<sizeLocal)                    //使用两层循环，一层上传，一层检测数据
    {
        if (this->currentMsg.status ) {   //被暂停或终止的项目
            free(fileName);
            SendCommand(sock,"ABOR\r\n");
            SendCommand(sock,QUIT);
            cout << "项目被暂停！" << endl;
            return false;
        }
        msleep(300);
        upSize=filesize(sock,fileName);
    }
    file.close();   //清理现场
    free(fileName);
    string s = closeDataSock(sock, dataSock);     //关闭数据端口
    SendCommand(sock, QUIT);
    return true;
}