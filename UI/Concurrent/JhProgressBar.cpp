//
// Created by ye11 on 2020/4/5.
//

#include "JhProgressBar.h"

JhProgressBar::JhProgressBar(QWidget* parent):
   QProgressBar(parent)
{

}

void JhProgressBar::setId(int id) {
    this->id=id;
}

int JhProgressBar::getId() {
    return this->id;
}