//
// Created by ye11 on 2020/3/20.
//

#ifndef WINFTP_FTPDATATHREAD_H
#define WINFTP_FTPDATATHREAD_H

#include "utils/utils.h"
#include "UI/mainwindow.h"
#include "ftpsock/download.h"
#include <QThread>

class FTPDataThread :public QThread{
    Q_OBJECT
public:
    SOCKET  commandSocket;            //通信socket
    string  storePath;                //存储在服务器的位置
    FTPDataThread(QObject* parent);
    vector<File> fileList;
    vector<int>  fileProcess;
    void run();
    void setupList(vector<File> list); //深度拷贝一份数据
    void setupPath(string path);
signals:
    void  valueChanged(const QVariant &value,const QVariant &index);
    void  fileFinished(const QVariant &finish,const QVariant &index);
    void  fileFailed(const QVariant&failed,const QVariant &index);
};


#endif //WINFTP_FTPDATATHREAD_H
