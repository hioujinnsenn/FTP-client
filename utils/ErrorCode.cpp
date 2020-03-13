//
// Created by ye11 on 2020/3/13.
//

#include "ErrorCode.h"
/**
 * 没有支持多进程和多线程，不支持同时访问
 * 返回错误码说明：  0：没有错误
 *                 -1：账号密码错误
 *                 -2：网络和服务器ip地址错误
 *
 * */
int ErrorCode::getCode() {

    if(codeQueue.size()>0) {
        int p = codeQueue.front();
        codeQueue.pop();
        return p;
    }
    else {
        return 0;
    }
}
void ErrorCode::setCode(int  code) {
    codeQueue.push(code);
}