//
// Created by ye11 on 2020/4/9.
//

#ifndef WINFTP_FILEMSG_H
#define WINFTP_FILEMSG_H

#include <iostream>
using namespace std;
class FileMsg{
public:
    //======== 主线程向子线程传递数据的主要两个项目 =========//
    string filepath;     //完整路径,对于目录而言，这个就是目录路径
    int id;              //下载任务的唯一id值
    //===================================================//
    //   单文件断点续传
    int UpOrDown ;       //上传或者下载，0为上传，1为下载，2为上传的断点续传，3为下载的断点续传
    int status;          //1暂停还是0下载中，通过切换这个值，实现暂停项的继续下载，暂停后恢复就变成断点续传了
    string storepath;    //数据传输的存储方的路径，目录断点续传也有用
    int  isDir;          //1:目录，0:文件
    //===================================================//
    //    目录断点续传
    int finishNum=0;       // 已经处理完的文件数目，同时也等于当前正在处理的文件的编号

};


#endif //WINFTP_FILEMSG_H
