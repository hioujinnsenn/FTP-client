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
#include <shlwapi.h>
using namespace std;

string upload(SOCKET sock, string uploadPath, string filePath);

bool uploadFile(SOCKET sock, string uploadPath, string filePath);

bool uploadDir(SOCKET sock, string uploadPath, string dirPath);


#endif //WINFTP_UPLOAD_H
