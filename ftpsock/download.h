//
// Created by ye11 on 2020/2/29.
//

#ifndef WINFTP_DOWNLOAD_H
#define WINFTP_DOWNLOAD_H

#include <ftpsock/ftpdata.h>
#include <utils/utils.h>
#include <fstream>
int download(SOCKET socket,string Path,string storePath);
#endif //WINFTP_DOWNLOAD_H
