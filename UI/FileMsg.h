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
    string filepath;     //完整路径
    int id;              //下载任务的唯一id值
    //===================================================//
    long filesize;     //已经上传的文件大小，默认是0，断点续传下有意义
    string filename;     //单纯的文件名
    int UpOrDown ;       //上传或者下载，0为上传，1为下载，2为上传的断点续传，3为下载的断点续传
    int status;          //1暂停还是0下载中，通过切换这个值，实现暂停项的继续下载，暂停后恢复就变成断点续传了
    string storepath;    //数据传输的存储方的路径
    int  isDir;          //1:目录，0:文件
    string  Dir_Downloading_File;  //当一个项目是目录时，记录对应的下载中的那个文件的路径
};


#endif //WINFTP_FILEMSG_H
