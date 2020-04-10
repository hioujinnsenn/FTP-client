//
// Created by ye11 on 2020/3/20.
//

#include "../../ui_mainwindow.h"
#include "UI/mainwindow.h"


// NOTE ：本地地址栏，跳转是按return
void MainWindow::on_lineEdit_returnPressed()
{
    QDir dir(local_pwd.data());
    string newAddress=ui->lineEdit->text().toStdString();
    int p=dir.cd(newAddress.data());   //如果是一个目录无论是一个相对目录还是绝对目录，cd成功返回1. 失败则返回0
    if(p==1)
    {
        QFileInfoList qlist = dir.entryInfoList();
        QFileInfoListToVector(&qlist, &localList);
        setupList(ui->listWidget1_1, ui->listWidget1_2, ui->listWidget1_3, localList);
        this->update();
        // IMPORTANT ： 要再次写回
        local_pwd = dir.path().toStdString();
        emit send_local_path(local_pwd);
    } else{
        ui->lineEdit->setText("");
        ui->lineEdit->setPlaceholderText("请检查地址是否正确");
    }
}

// note :本地端滑动条
void MainWindow::on_list1_3_scrollBar_value_changed(int action) {
    ui->listWidget1_1->verticalScrollBar()->setValue(action);
    ui->listWidget1_2->verticalScrollBar()->setValue(action);
}

//对文件列表进行选择时的回调函数
void MainWindow::on_listWidget1_1_itemSelectionChanged()
{
    QList<QListWidgetItem*> list=this->ui->listWidget1_1->selectedItems();
    for(int  i=0;i<list.size();i++)
    {
        cout<<list[i]->text().toStdString()<<endl;
    }
}

// 本地列表双击
void MainWindow::on_listWidget1_1_itemDoubleClicked(QListWidgetItem *item) {
    string text=item->text().toStdString();
    QDir dir(local_pwd.data());
    int p=dir.cd(text.data());
    if(p==1)
    {
        //是一个目录的话并且切换成功的话
        QFileInfoList qlist=dir.entryInfoList();
        QFileInfoListToVector(&qlist,&localList);
        setupList(ui->listWidget1_1,ui->listWidget1_2,ui->listWidget1_3,localList);
        this->update();
        // IMPORTANT ： 要再次写回
        local_pwd=dir.path().toStdString();
        emit send_local_path(local_pwd);
    }
}

// NOTE 文件列表单击事件处理,设置可以被编辑
void MainWindow::on_listWidget1_1_itemClicked(QListWidgetItem *item)
{
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    //设置可编辑
    local_lastItemName=item->text().toStdString();
}

// 本地地址栏编辑
void MainWindow::on_local_list_edited(QWidget *editor) {
    string NewText=((QLineEdit*)editor)->text().toStdString();
    QListWidgetItem *current=ui->listWidget1_1->currentItem();

    //
    if(current->data(Qt::UserRole+1).toInt()==1)
    {
        //如果是新建的一个项目
        QDir dir(local_pwd.data());
        dir.mkdir(NewText.data());
        LocalRefresh();
        return ;
    }
    if(NewText.size()>0)
    {
        //重命名文件
        QDir dir(local_pwd.data());
        dir.rename(local_lastItemName.data(),NewText.data());
        LocalRefresh();
    }
}


/**
 * @details 刷新本地文件窗口的函数
 *          使用时机：点击刷新按钮、重命名文件、下载文件完成
 * */
void MainWindow::LocalRefresh() {

    QDir dir(local_pwd.data());
    QFileInfoList qlist=dir.entryInfoList();
    QFileInfoListToVector(&qlist,&localList);
    setupList(ui->listWidget1_1,ui->listWidget1_2,ui->listWidget1_3,localList);
    this->update();

}

//
//  本地刷新按钮
//
void MainWindow::on_pushButton_clicked()
{
    LocalRefresh();
}
