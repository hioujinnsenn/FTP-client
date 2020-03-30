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
using namespace std;

class uploadThread: public QThread
{
    Q_OBJECT
public:
    uploadThread(SOCKET sock, vector<string> filePath);
    ~uploadThread() override;
    string upload(SOCKET sock, string filePath);
    bool uploadFile(SOCKET sock, string filePath);
    bool uploadDir(SOCKET sock, string dirPath);
protected:
    void run() override;
private:
    vector<string> filePath;
    SOCKET sock;
    bool isDir; //标记当前上传的是文件还是文件夹
signals:
    void sendProgress(int progress);
};

#endif //FTP_CLIENT_UPLOAD_QTHREAD_H
