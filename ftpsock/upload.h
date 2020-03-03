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
using namespace std;

string uploadFile(SOCKET sock, string uploadPath, string filePath);

string uploadDir(SOCKET sock, string uploadPath, string dirPath);


#endif //WINFTP_UPLOAD_H
