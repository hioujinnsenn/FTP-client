//
// Created by asus on 2020/2/27.
//

#ifndef FTP_CLIENT_COMMANDS_H
#define FTP_CLIENT_COMMANDS_H

#include "utils.h"
#include <ftpsock/ftpdata.h>

SOCKET pasv(SOCKET sock);

string closeDataSock(SOCKET sock, SOCKET dataSock);
#endif //FTP_CLIENT_COMMANDS_H
