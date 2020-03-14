//
// Created by ye11 on 2020/3/13.
//

#ifndef WINFTP_ERRORCODE_H
#define WINFTP_ERRORCODE_H

#include <queue>
using namespace std;
class ErrorCode {

 private:queue<int> codeQueue;
 public:
    int getCode();
    void setCode(int code);
};


#endif //WINFTP_ERRORCODE_H
