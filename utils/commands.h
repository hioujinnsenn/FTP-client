//
// Created by asus on 2020/2/27.
//

#ifndef FTP_CLIENT_COMMANDS_H
#define FTP_CLIENT_COMMANDS_H

#include "utils.h"
#include <ftpsock/ftpdata.h>
#include <fstream>
#include <shlwapi.h>
#include "ftpsock/login.h"
#define APPE  "APPE %s\r\n"

SOCKET pasv(SOCKET sock);                             //开启数据端口，开启命令端口的操作不在这里进行

string closeDataSock(SOCKET sock, SOCKET dataSock);

size_t getFileSize(string filePath);

size_t stringToSize_t(string s);

void getAllFiles(string dirPath, vector<string> &files, vector<string> &names);

string mkd(SOCKET sock, string path );

#endif //FTP_CLIENT_COMMANDS_H
