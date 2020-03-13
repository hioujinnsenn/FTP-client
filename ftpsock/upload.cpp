//
// Created by asus on 2020/2/24.
//
#include "upload.h"

/***
 * 上传的主函数
 * @param sock 控制端口
 * @param uploadPath 需要上传到的目录路径（必须是绝对路径）
 * @param filePath 所需上传的文件所在本地路径
 * @return 返回参数可修改，目前为string
 */
string upload(SOCKET sock, string uploadPath, string filePath)
{
    bool result;
    if(PathIsDirectory(filePath.data())){   //是目录路径
        result=uploadDir(sock, uploadPath, filePath);
    }
    else{   //是文件路径
        result=uploadFile(sock, uploadPath, filePath);
    }
    if(result){
        return "文件成功上传！";
    }
    return "文件上传出错！";
}

/***
 * 上传功能函数，上传本地文件到服务器指定路径位置
 * @param sock
 * @param uploadPath
 * @param filePath
 * @return 执行的结果以字符串形式返回并打印
 * offset参数可作为上传进度的参考值，与sizeLocal的比值即可判断上传进度
 */
bool uploadFile(SOCKET sock, string uploadPath, string filePath)     //上传文件到服务器
{
    SendCommand(sock, "TYPE i\r\n");
    SOCKET dataSock=pasv(sock);   //开启被动模式，返回数据端口socket
    if(! uploadPath.empty()){    //若没有指定目录，则不需要change directory
        string r=cwd(sock, uploadPath);   //改变服务器工作目录
        if(r.substr(0,3)>"300"){    //该目录不存在
            mkd(sock, uploadPath);  //创建目录
            cwd(sock, uploadPath);  //进入目录
        }
    }
    char* fileName=(char*)malloc(filePath.size()); //所需上传文件的文件名
    memset(fileName, 0,filePath.size());
    int i=(filePath.find_last_of('\\')!=string::npos) ? filePath.find_last_of('\\') : filePath.find_last_of('/');  //找到文件名前的分隔符
    sprintf(fileName, filePath.substr(i+1, string::npos).c_str());  //得到所需上传的文件名
    size_t offset=0;  //文件写偏移量，用于断点续传。文件内容格式：一个断点文件一行，【文件路径 上传偏移量】中间空格隔开

    fstream logFile("upload.log");  //用于记录上传断点情况，例如手动停止、断网等情况
    size_t logPosition=0; //日志中文件所在的位置
    if(! logFile.is_open()){    //若该文件记录不存在，offset=0
        string size=SendCommand(sock,SIZE, fileName);   //用size命令判断要上传的文件是否已经在目录中
        if(size.substr(0, 3)=="213") {   //有重名文件eg："213 7900000\r\n"否则505
            string r = "文件已存在或重名！是否覆盖？（0取消上传，1覆盖当前文件）";
            cout << r << endl;
            int operation;
            cin >> operation;
            if (!operation) {
                free(fileName);
                closesocket(dataSock);
                return false;
            }
        }
        logFile.open("upload.log", ios::out);   //文件不存在则创建
    }
    else{   //存在日志文件则查找该文件是否需要续传
        while(!logFile.eof()){
//            logPosition=logFile.tellg();
            char* buffer=(char*)malloc(Dlength);
            logFile.getline(buffer, Dlength);
            string line=buffer;
            int p=line.find(filePath.c_str());
            if(p!=-1){
                offset=stoul(line.substr(p+filePath.size()+1) );
                logPosition=logFile.tellg();
                logPosition-=line.size();
                break;
            }
        }
    }
    size_t sizeLocal=getFileSize(filePath);     //得到本地需要上传的文件的大小

    ifstream file(filePath, ios::binary);  //需要上传的文件流
    if(! file.is_open()){
        string r("文件打开失败！");
        cout<<r<<endl;
        closesocket(dataSock);
        return false;
    }
    file.seekg(offset, ios::beg);   //将file流的读取指针移到上传的文件内容后面，继续上传
    string mes=SendCommand(sock, APPE, fileName);  //请求上传文件，若不存在则新建，反之则在文件中接着上传
    string m=mes.substr(0,2);
    if(m>"300" || m.empty()){
        cout<<"文件上传失败"<<endl;
        file.close();
        free(fileName);
        closesocket(dataSock);
        return false;   //失败返回
    }
    while(! file.eof()){    //在到达文件末尾前持续读文件，将文件内容通过数据端口上传到服务器
        char* message=(char*)malloc(Dlength);   //dataBuffer
        memset(message, 0, Dlength);
        if(offset+Dlength>sizeLocal){
            size_t rlength=sizeLocal-offset;
            file.read(message, rlength+1);  //多读一个字节，判断已不能读，使得eof为true，从而避免死循环问题。否则永远无法多读一位，使eof为true
            send(dataSock, message, rlength, 0);
            offset+=rlength;
        }
        else{
            file.read(message, Dlength);
            send(dataSock, message, Dlength, 0);
            offset+=Dlength;
        }
        free(message);
    }
    file.close();   //清理现场
    free(fileName);
    string s=closeDataSock(sock, dataSock);     //关闭数据端口
    cout<<s;
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
bool uploadDir(SOCKET sock, string uploadPath, string dirPath)    //上传文件夹到服务器指定目录下
{
    vector<string> files, names;
    string dirName=dirPath.substr(dirPath.find_last_of("\\")+1, dirPath.size());
    string r=cwd(sock, uploadPath+dirName);
    if(r.substr(0,3)=="250"){   //目录名重名或已存在
        cout<<"目录重名或已存在!"<<endl;
        return false;
    }
    getAllFiles(dirPath, files, names); //得到目录下所有的文件路径和名称，两者顺序对应
    if(files.size()!=names.size()){
        cout<<"查找目录子文件出错！路径数和文件名数不等"<<endl;
        return false;
    }
    for(int i=0; i<files.size(); i++){
        string path(files.at(i));   //当前文件路径
        string name(names.at(i));   //当前文件名
        int pos=dirPath.find_last_of("\\");
        string wd=uploadPath+"\\"+path.substr(pos+1, path.size()-name.size()-pos-2);   //得到文件所在路径
        if(! uploadFile(sock, wd, path))
            return false;
    }
    return true;
}