//
// Created by ye11 on 2020/2/22.
//

#ifndef WINFTP_LOGIN_H
#define WINFTP_LOGIN_H

#include "utils/ErrorCode.h"
#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#define clength 400
#define dlength 1000
extern  ErrorCode code;
SOCKET login(char* username,char*  password);
SOCKET login(char* username,char* password,char* ip);
#endif //WINFTP_LOGIN_H
