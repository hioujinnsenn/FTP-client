//
// Created by ye11 on 2020/4/5.
//

# include "JhButton.h"

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
