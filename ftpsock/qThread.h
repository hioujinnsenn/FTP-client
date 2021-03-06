//
// Created by asus on 2020/3/20.
//

#ifndef FTP_CLIENT_QTHREAD_H
#define FTP_CLIENT_QTHREAD_H

#include <QThread>
#include <winsock.h>
#include "utils/utils.h"
#include "utils/commands.h"
#include <cstdio>
#include <string>
#include<fstream>
#include <io.h>
#include <queue>
#include <deque>
#include "UI/FileMsg.h"
#include <QFile>
#include <ftpsock/ftpdata.h>
#include <utils/utils.h>
using namespace std;
/***
 * 实际上下载和上传应该同一个了，没有区分了才对
 */
class qThread: public QThread
{
Q_OBJECT
private:

    char* Username;
    char* Password;
    char* Ip;
    vector<FileMsg> msgs;
    vector<FileMsg> stopedMsgs;
    FileMsg currentMsg;     // IMPORTANT 正在传输中的任务的Msg，通过改变该元素的值，实现暂停
    int id;         //当前任务的id
    int nextId;     //下一个任务的id，
    bool thread_alive=true;
    string remote_path;       //备份的远程目前的路径，需要恢复
    string local_path;        //下载默认就是下载到选择的路径
public:
    qThread(string username,string password,string ip);
    ~qThread() override;
    bool download(string filePath,int id);
    long filesize(SOCKET sock ,char* filename);
    bool upload(string filePath,int id);
    bool uploadFile(string filePath,int id);      // 单个文件以文件的
    bool uploadDir(string dirPath,int id);        // 文件夹的上传处理，以文件的个数作为进度的依据
    bool uploadDirFile(string filePath, int id, string uploadPath);    //文件夹中单个文件上传，指定目录
    bool uploadContinue();  //上传的断点续传
    bool uploadFileContinue(string filePath, string storePath);
    bool uploadDirContinue(string dirPath, string storePath, int filecount);
    bool uploadDirFileContinue(string filePath, string uploadPath);

    bool downloadFile(string filePath,int id);     // 单个文件的下载
    bool isDirRemote(string filePath);  //判断服务器端的路径是文件还是文件夹
    //用于文件夹的下载
    int downloadDirFile(string filePath,string storePath,int filecount); // 单个文件的下载，指定本地路径
    bool downloadDir(string filePath,int id);      // 文件夹的下载处理
    bool downloadFileContinue(string filePath,string storePath,long  downloadsize);  //文件断点续传
    bool downloadDirContinue(string DirPath,string storePath,int finishedNum,int id);    //目录断点续传
    bool downloadContinue();
    int downloadDirStopedFileContinue(string filePath,string storePath,int filecount,long downloadsize);

//    bool deleteRemote(string fi)
protected:
    void run() override;
signals:
    void sendProgress(int progress, int id);
    void finishOne(int id, int nextId, int isUpload);
    void send_Dir_filecount(int filecount,int id );        //信号，回传到UI界面，文件夹目录的大小
    void sendStateChange(int id, int state, int isUpload);    //传递状态参数给前端
public slots:
    void receive_filemsg(FileMsg msg);
    void receive_remote_path(string path); // 由于使用的方式是每次自行登陆获取sock，导致每次操作回到根目录
    // 需要从UI界面拿回local_path
    void  receive_local_path(string path);
    void  receive_pause_id(int id);         //接受暂停的任务的id，并暂停该任务
    void receive_cancel_id(int id);     //接收终止任务的id，并删除对应的项目界面和对应的文件
};

#endif //FTP_CLIENT_QTHREAD_H