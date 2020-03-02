//
// Created by asus on 2020/2/27.
//

#ifndef FTP_CLIENT_COMMANDS_H
#define FTP_CLIENT_COMMANDS_H

#include "utils.h"
#include <ftpsock/ftpdata.h>
#include <fstream>

#define APPE  "APPE %s\r\n"

SOCKET pasv(SOCKET sock);

string closeDataSock(SOCKET sock, SOCKET dataSock);

size_t getFileSize(string filePath);

size_t stringToSize_t(string s);

#endif //FTP_CLIENT_COMMANDS_H
