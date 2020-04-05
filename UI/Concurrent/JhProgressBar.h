//
// Created by ye11 on 2020/4/5.
//

#ifndef WINFTP_JHPROGRESSBAR_H
#define WINFTP_JHPROGRESSBAR_H

#include <QWidget>
#include <QProgressBar>
namespace Ui {
    class  JhProgressBar;
}
class JhProgressBar:public QProgressBar {

public :
    JhProgressBar(QWidget*parent);
    int id;  //下载的任务唯一id，跟同一个widget下的button同一个id
    void setId(int id);
    int getId();
};


#endif //WINFTP_JHPROGRESSBAR_H
