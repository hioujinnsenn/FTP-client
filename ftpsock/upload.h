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
//#ifndef clength
//#define clength 1000
//#endif

string upload(SOCKET socket, string dirName, string fileName);

#endif //WINFTP_UPLOAD_H
