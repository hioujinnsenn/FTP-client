//
// Created by ye11 on 2020/4/9.
//

#ifndef WINFTP_FILEMSG_H
#define WINFTP_FILEMSG_H

#include <iostream>
using namespace std;
class FileMsg{
public:
    string filename;     //单纯的文件名
    string filepath;     //完整路径
    string filesize;     //已经上传的文件大小，默认是0，断点续传下有意义
    int id;              //下载任务的唯一id值
    int UpOrDown ;       //上传或者下载，0为上传，1为下载，2为上传的断点续传，3为下载的断点续传
    int status;          //暂停还是下载中，通过切换这个值，实现暂停项的继续下载，暂停后恢复就变成断点续传了
    string storepath;    //数据传输的存储方的路径
};


#endif //WINFTP_FILEMSG_H
