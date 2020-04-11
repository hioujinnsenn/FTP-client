//
// Created by ye11 on 2020/4/5.
//

#ifndef WINFTP_JHBUTTON_H
#define WINFTP_JHBUTTON_H

#include <QPushButton>
namespace Ui {
    class JhButton;
}

class JhButton: public QPushButton
{
     Q_OBJECT;
protected:
    void mousePressEvent(QMouseEvent* event);
public :
    JhButton(QWidget* parent);
    int id;           //作为下载任务的唯一标识
    int ButtonType;   //自定义按钮要完成的任务：0：暂停，1：终止  //具体再完善
    void setId(int id);
    int getId();
    int state=0;  //标记按钮状态（暂停=1/继续=0
signals:
    void  send_pause_id(int id);      //区分：响应暂停，传送id
    void  send_cancel_id(int id);     //区分：响应取消，传送id

};

#endif //WINFTP_JHBUTTON_H
