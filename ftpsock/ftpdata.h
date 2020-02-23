//
// Created by ye11 on 2020/2/22.
// 用于处理后台部分跟数据相关的
// 包括打开一个获取数据口的位置
// 打开一个数据端口
// 数据传输、断点续传
//

#ifndef WINFTP_FTPDATA_H
#define WINFTP_FTPDATA_H

#include <iostream>
#include <winsock.h>
/*function： 将PASV命令获取到手的信息中提取出后面的两个端口数据*/
int getNewPort(std::string port);
SOCKET getNewSocket(const  char* addr,int port);
#endif //WINFTP_FTPDATA_H
