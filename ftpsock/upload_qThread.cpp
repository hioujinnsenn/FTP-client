//
// Created by asus on 2020/3/20.
//

#include "upload_qThread.h"
#include <QDebug>
#include "ftpsock/upload.h"

uploadThread::uploadThread(string username,string password,string ip) //构造器
{
    // 只传递必要的信息，文件信息通过信号槽传递
    this->Username=(char*)malloc(400);
    this->Password=(char*)malloc(400);
    this->Ip=(char*)malloc(400);
    memset(this->Username,0,400);
    memset(this->Password,0,400);
    memset(this->Ip,0,400);
    sprintf(this->Username,username.data());
    sprintf(this->Password,password.data());
    sprintf(this->Ip,ip.data());
}

uploadThread::~uploadThread()   //析构器
{
    qDebug()<<"uploadThread::~uploadThread";
}

void uploadThread::setStop()
{
    if(state==0){
        state=1;    //进度暂停
    }
    else if(state==1){
        state=0;    //进度继续（开始）
    }
}
void uploadThread::receive_filemsg(FileMsg msg)   //  接受UI界面传递过来的文件信息
{
    this->ids.push(msg.id);
    this->filePath.push(msg.filepath);
}
void uploadThread::run()             // 此进程修改成和UI主界面共生存在
{
   while(thread_alive)               // 应该设置成无限循环，不停止,通过设置thread_alive实现终止线程
   {
       int size=filePath.size();
       for (int i = 0; i < size; i++) {
           // 每次取队列头，然后处理完再把头扔了
           id=ids.front();
           ids.pop();
           cout<<"========================="<<endl;
           cout<<endl;
           cout<<id<<endl;
           cout<<endl;
           cout<<"========================="<<endl;
           if (i < size - 1)
               nextId = ids.front();
           else nextId = -1;
           string path = filePath.front();
           filePath.pop();
           SOCKET  sock=login(this->Username,this->Password,this->Ip);
           cwd(sock,this->remote_path);
           upload(sock, path, id);
           emit(finishOne(id, nextId));        //任务为一个文件则发送一个任务完成
           SendCommand(sock,QUIT);
       }
   }
}

/***
 * 上传的主函数
 * @param sock 控制端口
 * @param uploadPath 需要上传到的目录路径（必须是绝对路径）
 * @param filePath 所需上传的文件所在本地路径
 * @return 返回参数可修改，目前为string
 */
bool uploadThread::upload(SOCKET sock, string filePath,int id)
{
    bool result;
    if(PathIsDirectory(filePath.data())){   //是目录路径
        isDir=true;
        result=uploadDir(sock, filePath,id);
    }
    else{   //是文件路径
        isDir=false;
        result=uploadFile(sock, filePath,id);
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
bool uploadThread::uploadFile(SOCKET sock, string filePath,int id)     //上传文件到服务器
{
    SendCommand(sock, "TYPE i\r\n");

    SOCKET dataSock=pasv(sock);   //开启被动模式，返回数据端口socket
//    if(! uploadPath.empty()){    //若没有指定目录，则不需要change directory
//        string r=cwd(sock, uploadPath);   //改变服务器工作目录
//        if(r.substr(0,3)>"300"){    //该目录不存在
//            mkd(sock, uploadPath);  //创建目录
//            cwd(sock, uploadPath);  //进入目录
//        }
//    }
    char* fileName=(char*)malloc(filePath.size()); //所需上传文件的文件名
    memset(fileName, 0,filePath.size());
    int i=(filePath.find_last_of('\\')!=string::npos) ? filePath.find_last_of('\\') : filePath.find_last_of('/');  //找到文件名前的分隔符
    sprintf(fileName, filePath.substr(i+1, string::npos).c_str());  //得到所需上传的文件名
    size_t offset=0;  //文件写偏移量，用于断点续传。文件内容格式：一个断点文件一行，【文件路径 上传偏移量】中间空格隔开
//    int progress=0;     //通过offset/

//    ifstream logFile_i("upload.log");  //用于记录上传断点情况，例如手动停止、断网等情况
//    size_t logPosition=0; //日志中文件所在的位置
//    if(! logFile_i.is_open()){    //若该文件记录不存在，offset=0
//        string size=SendCommand(sock,SIZE, fileName);   //用size命令判断要上传的文件是否已经在目录中
//        if(size.substr(0, 3)=="213") {   //有重名文件eg："213 7900000\r\n"否则505
//            string r = "文件已存在或重名！是否覆盖？（0取消上传，1覆盖当前文件）";
//            cout << r << endl;
//            int operation;
//            cin >> operation;
//            if (!operation) {
//                free(fileName);
//                closesocket(dataSock);
//                return false;
//            }
//            else{
//                del(sock, fileName);    //覆盖文件则先删掉该文件
//            }
//        }
//    }
//    else{   //存在日志文件则查找该文件是否需要续传
//        while(!logFile_i.eof()){
////            logPosition=logFile.tellg();
//            char* buffer=(char*)malloc(Dlength);
//            logFile_i.getline(buffer, Dlength);
//            string line=buffer;
//            int p=line.find(filePath.c_str());
//            if(p!=-1){
//                offset=stoul(line.substr(p+filePath.size()+1) );
//                logPosition=logFile_i.tellg();
//                logPosition-=line.size();
//                break;
//            }
//        }
//    }
    size_t sizeLocal=getFileSize(filePath);     //得到本地需要上传的文件的大小
//    ofstream logFile_o("upload.log");
//    ifstream file(filePath, ios::binary);  //需要上传的文件流
    QFile file(QString::fromStdString(filePath));
    if(! file.open(QIODevice::ReadOnly)){
        string r("文件打开失败！");
        cout<<filePath<<r<<endl;
        closesocket(dataSock);
        return false;
    }
    file.seek(offset);
//    file.seek(offset, ios::beg);   //将file流的读取指针移到上传的文件内容后面，继续上传
    string mes=SendCommand(sock, APPE, fileName);  //请求上传文件，若不存在则新建，反之则在文件中接着上传
    string m=mes.substr(0,2);
    if(m>"300" || m.empty()){
        cout<<"文件上传失败"<<endl;
        file.close();
        free(fileName);
        closesocket(dataSock);
        return false;   //失败返回
    }
    long upSize=filesize(sock,fileName);
    while(upSize<sizeLocal)                    //使用两层循环，一层上传，一层检测数据
    {
        while (!file.atEnd()) {    //在到达文件末尾前持续读文件，将文件内容通过数据端口上传到服务器
            if (state == 1) {   //被暂停的项目
                /***
                 * 将方法getFilePath得到的没传完的文件路径返回到主窗口的槽函数中，将没写完的当前文件加入断点续传日志中。
                 * 余下内容同终止状态
                 */
                cout << "项目被暂停！" << endl;
            }
            if (state) {   //该项目被终止或暂停，则需要销毁线程
                /***
                 * 此处空出来后面再写，要完成的内容有终止线程、删除上传了一半的文件/文件夹
                 */
                cout << "项目被终止！（暂停目前也会经过这句）" << endl;
            }
            char *message = (char *) malloc(Dlength);   //dataBuffer
            memset(message, 0, Dlength);
            size_t rlength = file.read(message, Dlength); //read返回当前读到的字节数
            send(dataSock, message, rlength, 0);
            offset += rlength;
//        if(offset+Dlength>sizeLocal){
//            size_t rlength=sizeLocal-offset;
//            file.read(message, rlength+1);  //多读一个字节，判断已不能读，使得eof为true，从而避免死循环问题。否则永远无法多读一位，使eof为true
//            send(dataSock, message, rlength, 0);
//            offset+=rlength;
//        }
//        else{
//            file.read(message, Dlength);
//            send(dataSock, message, Dlength, 0);
//            offset+=Dlength;
//        }
            free(message);
            upSize=filesize(sock,fileName);
            if (!isDir) {
                emit sendProgress(int(100 * (upSize / (sizeLocal*1.0) )), id);    //将文件发送进度
            }

        }
        upSize=filesize(sock,fileName);
        if (!isDir) {
            emit sendProgress(int(100 * (upSize / (sizeLocal*1.0) )), id);    //将文件发送进度
        }
    }

    file.close();   //清理现场
    free(fileName);
    string s=closeDataSock(sock, dataSock);     //关闭数据端口
    cout<<s;
    flush(cout);
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
bool uploadThread::uploadDir(SOCKET sock, string dirPath,int id)    //上传文件夹到服务器指定目录下
{
    string uploadPath=pwd(sock);    //得到需要上传到的目录
    vector<string> files, names;
    string dirName=dirPath.substr(dirPath.find_last_of("\\")+1, dirPath.size());
//    string r=cwd(sock, uploadPath+dirName);
//    if(r.substr(0,3)=="250"){   //目录名重名或已存在
//        cout<<"目录重名或已存在!"<<endl;
//        return false;
//    }
    getAllFiles(dirPath, files, names); //得到目录下所有的文件路径和名称，两者顺序对应
    if(files.size()!=names.size()){
        cout<<"查找目录子文件出错！路径数和文件名数不等"<<endl;
        return false;
    }
    for(int i=0; i<files.size(); i++){
        string path(files.at(i));   //当前文件路径
        string name(names.at(i));   //当前文件名
        int pos=dirPath.find_last_of("/");
        if(pos==-1) pos=dirPath.find_last_of("\\");
        string wd;
        if(uploadPath=="\\" || uploadPath=="/")
            wd=uploadPath+path.substr(pos+1, path.size()-name.size()-pos-2);   //得到文件所在路径
        else  wd=uploadPath+"/"+path.substr(pos+1, path.size()-name.size()-pos-2);
        string r=cwd(sock, wd);   //改变服务器工作目录
        if(r.substr(0,3)>"300"){    //该目录不存在
            mkd(sock, wd);  //创建目录
            cwd(sock, wd);  //进入目录
        }
        if(! uploadFile(sock, path,id))
            return false;
        emit sendProgress(100*(i+1)/files.size(), id);    //上传目录的进度
    }
    return true;
}

long uploadThread::filesize(SOCKET sock,char* filename){
    char*  path=(char*)malloc(Dlength);
    memset(path,0,Dlength);
    sprintf(path,filename);
    string p=SendCommand(sock,SIZE,path);
    int i=p.find_last_of(' ');
    string size=p.substr(i+1,(p.size()-i-3));  //p.size()-2 -(i+1)
    istringstream  is(size);
    long sizel;
    is>>sizel;
    free(path);
    return sizel;
}

void uploadThread::receive_remote_path(string path) {
    this->remote_path=path;
}