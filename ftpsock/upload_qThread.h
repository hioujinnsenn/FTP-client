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
#include <UI/mainwindow.h>

using namespace std;
/***
 * 需要
 */
class uploadThread: public QThread
{
    Q_OBJECT
private:
    vector<string> filePath;
    SOCKET sock;
    bool isDir; //标记当前上传的是文件还是文件夹
    int state=0; //标记当前进度状态，0为继续，1为暂停，2为终止
    vector<int> ids; //当前线程所有任务id号列表
    int id; //当前任务的id
    int nextId; //下一个任务的id，
public:
    uploadThread(SOCKET sock, vector<string> filePath, vector<int> ids);
    ~uploadThread() override;
    bool upload(SOCKET sock, string filePath);
    bool uploadFile(SOCKET sock, string filePath);
    bool uploadDir(SOCKET sock, string dirPath);
//    vector<string> getFilePath();
protected:
    void run() override;
signals:
    void sendProgress(int progress, int id);
    void finishOne(int id, int nextId);

public slots:
     void setStop();
};

#endif //FTP_CLIENT_UPLOAD_QTHREAD_H
