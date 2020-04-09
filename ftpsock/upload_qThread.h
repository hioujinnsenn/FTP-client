//
// Created by asus on 2020/3/20.
//

#ifndef FTP_CLIENT_UPLOAD_QTHREAD_H
#define FTP_CLIENT_UPLOAD_QTHREAD_H

#include <QThread>
#include <winsock.h>
#include "utils/utils.h"
#include "utils/commands.h"
#include <cstdio>
#include <string>
#include<fstream>
#include <io.h>
#include <shlwapi.h>
#include <queue>
#include "UI/FileMsg.h"
#include <QFile>
using namespace std;
/***
 * 实际上下载和上传应该同一个了，没有区分了才对
 */
class uploadThread: public QThread
{
    Q_OBJECT
private:

    char* Username;
    char* Password;
    char* Ip;
    bool isDir;     //标记当前上传的是文件还是文件夹
    int state=0;    //标记当前进度状态，0为继续，1为暂停，2为终止
    queue<FileMsg> msgs;
    int id;         //当前任务的id
    int nextId;     //下一个任务的id，
    bool thread_alive=true;
    string remote_path;       //备份的远程目前的路径，需要恢复
public:
    uploadThread(string username,string password,string ip);
    ~uploadThread() override;
    long filesize(SOCKET sock ,char* filename);
    bool upload(SOCKET sock, string filePath,int id);
    bool uploadFile(SOCKET sock, string filePath,int id);             //单个文件以文件的
    bool uploadDir(SOCKET sock, string dirPath,int id);        // 文件夹的上传处理，以文件的个数作为进度的依据

protected:
    void run() override;
signals:
    void sendProgress(int progress, int id);
    void finishOne(int id, int nextId);
public slots:
     void setStop();
     void receive_filemsg(FileMsg msg);
     void receive_remote_path(string path); // 由于使用的方式是每次自行登陆获取sock，导致每次操作回到根目录
    // 需要从UI界面拿回remote_path
};

#endif //FTP_CLIENT_UPLOAD_QTHREAD_H
