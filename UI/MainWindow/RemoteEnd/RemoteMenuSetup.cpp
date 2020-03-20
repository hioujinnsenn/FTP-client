//
// Created by ye11 on 2020/3/20.
//
#include "../../ui_mainwindow.h"
#include "UI/mainwindow.h"
void MainWindow::on_remoteMenu_addDir_triggered() {
    QListWidgetItem *item2=new QListWidgetItem(ui->listWidget2_2);
    item2->setText("");
    QListWidgetItem *item3=new QListWidgetItem(ui->listWidget2_3);
    item3->setText("系统文件夹");
    QListWidgetItem * item=new QListWidgetItem(ui->listWidget2_1);
    item->setIcon(QIcon(DIRICON));
    item->setText("请输入文件夹名称");
    item->setFlags(item->flags()|Qt::ItemIsEditable);
    //对item进行数据设置标新位
    QVariant isNew=1;
    item->setData(Qt::UserRole+1,isNew);
    ui->listWidget2_1->update();
    ui->listWidget2_1->scrollToItem(item);
    ui->listWidget2_1->setCurrentItem(item);
    ui->listWidget2_1->editItem(item);
}

void MainWindow::on_remoteMenu_delItem_triggered() {
   QListWidgetItem *item=ui->listWidget2_1->currentItem();
   string path=item->text().toStdString();
   cwd(CommandSocket,remote_pwd);
   if(item->data(Qt::UserRole+2).toInt()==1)
   {
       //目录
       rmd(CommandSocket,path);
   }
   else{
       del(CommandSocket,path);
   }
    char*  message=(char*)malloc(Dlength);
    memset(message,0,Dlength);
    recv(CommandSocket,message,Dlength,0);
    cout<<message;
    free(message);   //清理残余信息

    SOCKET datasock=pasv(CommandSocket);
    vector<File> serverList=ls(CommandSocket,datasock);
    File parent;
    parent.type=1;
    parent.name="..";
    parent.path="..";
    serverList.insert(serverList.begin(),parent);
    setupList(ui->listWidget2_1,ui->listWidget2_2,ui->listWidget2_3,serverList);

}