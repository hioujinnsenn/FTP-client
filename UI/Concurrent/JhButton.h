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

public :
    JhButton(QWidget* parent);
    int id;      //作为下载任务的唯一标识
    void setId(int id);
    int getId();

};

#endif //WINFTP_JHBUTTON_H
