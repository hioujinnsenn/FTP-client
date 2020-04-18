//
// Created by ye11 on 2020/4/5.
//

#include <iostream>
# include "JhButton.h"

// Fixed ：2020/4/10 增加button的点击传递id信号的能力
JhButton::JhButton(QWidget* parent):
  QPushButton(parent)
{
}

void JhButton::setId(int id) {
    this->id=id;
}

int JhButton::getId() {
    return this->id;
}


// 重写的鼠标点击函数
// 发生鼠标点击时，就发送信号
void JhButton::mousePressEvent(QMouseEvent *event) {

    if(this->ButtonType==0)
    {
        std::cout<<"点击了暂停/恢复"<<std::endl;
        QIcon icon;
        if(state==0){   //当前进度为上传/下载中
            state=1;
            icon=QIcon("../UI/resoucre/icon/48/continue.png");
        }
        else{
            state=0;
            icon=QIcon("../UI/resoucre/icon/48/stop.png");
        }
        this->setIcon(icon);    //改变图标
        emit send_pause_id(this->id);
    } else{
        emit  send_cancel_id(this->id);
    }
}