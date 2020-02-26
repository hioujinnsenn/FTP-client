//
// Created by asus on 2020/2/24.
//

#ifndef WINFTP_UPLOAD_H
#define WINFTP_UPLOAD_H
#include "utils/utils.h"
#include <cstdio>
#include <string>
#include<fstream>
//#ifndef clength
//#define clength 1000
//#endif

int upload(SOCKET socket, SOCKET dataSocket, string dirName, string fileName);

#endif //WINFTP_UPLOAD_H
