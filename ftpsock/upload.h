//
// Created by asus on 2020/2/24.
//
#ifndef WINFTP_UPLOAD_H
#define WINFTP_UPLOAD_H

#include "utils/utils.h"
#include "utils/commands.h"
#include <cstdio>
#include <string>
#include<fstream>
#include <io.h>
#include <windows.h>
//#include <sys/stat.h>
using namespace std;

string upload(SOCKET sock, string uploadPath, string filePath);

bool uploadFile(SOCKET sock, SOCKET dataSock, string filePath);

bool uploadDir(SOCKET sock, SOCKET dataSock, string dirPath);


#endif //WINFTP_UPLOAD_H
