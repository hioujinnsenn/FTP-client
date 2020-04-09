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
    string filesize;     //文件大小
    int id;              // 下载任务的唯一id值
    int UpOrDown ;       //上传或者下载，0为上传，1为下载
};


#endif //WINFTP_FILEMSG_H
