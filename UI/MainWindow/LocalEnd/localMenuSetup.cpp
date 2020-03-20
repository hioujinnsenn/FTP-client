//
// Created by ye11 on 2020/3/19.
//
//  右键菜单的设置
//

//这里可能编译前会提示错误，实际不会有问题
#include "../../ui_mainwindow.h"
#include "UI/mainwindow.h"
//点击右键菜单中的新建文件夹

void MainWindow::on_localMenu_addDir_triggered() {
    QListWidgetItem *item2=new QListWidgetItem(ui->listWidget1_2);
    item2->setText("");
    QListWidgetItem *item3=new QListWidgetItem(ui->listWidget1_3);
    item3->setText("系统文件夹");
    QListWidgetItem * item=new QListWidgetItem(ui->listWidget1_1);
    item->setIcon(QIcon(DIRICON));
    item->setText("请输入文件夹名称");
    item->setFlags(item->flags()|Qt::ItemIsEditable);
    //对item进行数据设置标新位
    QVariant isNew=1;
    item->setData(Qt::UserRole+1,isNew);
    ui->listWidget1_1->update();
    ui->listWidget1_1->scrollToItem(item);
    ui->listWidget1_1->setCurrentItem(item);
    ui->listWidget1_1->editItem(item);
}

