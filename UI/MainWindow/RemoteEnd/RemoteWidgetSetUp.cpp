//
// Created by ye11 on 2020/3/20.
//


#include "../../ui_mainwindow.h"
#include "UI/mainwindow.h"

// NOTE :list2_3是服务器端widget的第三个，也就是最后一个
void MainWindow::on_list2_3_scrollBar_value_changed(int action) {
    ui->listWidget2_1->verticalScrollBar()->setValue(action);
    ui->listWidget2_2->verticalScrollBar()->setValue(action);
}
//   远程刷新按钮
void MainWindow::on_pushButton_2_clicked()
{
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


void MainWindow::on_listWidget2_1_itemSelectionChanged()
{

}

//  远程窗口的列表项双击
void MainWindow::on_listWidget2_1_itemDoubleClicked(QListWidgetItem *item) {
    char*  message=(char*)malloc(Dlength);
    memset(message,0,Dlength);
    recv(CommandSocket,message,Dlength,0);
    cout<<message;
    free(message);   //清理残余信息
    string name=item->text().toStdString();
    string returnInfo =cwd(CommandSocket,name);
    SOCKET datasock=pasv(CommandSocket);
    vector<File> serverList=ls(CommandSocket,datasock);
    File parent;
    parent.type=1;
    parent.name="..";
    parent.path="..";
    serverList.insert(serverList.begin(),parent);
    setupList(ui->listWidget2_1,ui->listWidget2_2,ui->listWidget2_3,serverList);
    remote_pwd=pwd(CommandSocket);   //更新远程目录
}


void MainWindow::on_listWidget2_1_itemClicked(QListWidgetItem *item)
{
    item->setFlags(item->flags()|Qt::ItemIsEditable);
    remote_lastItemName=item->text().toStdString();      //远程当前选中名称
}

//远程端的item修改名称
void MainWindow::on_remote_list_edited(QWidget *editor) {


    string newName=((QLineEdit*)editor)->text().toStdString();
    QListWidgetItem * item=ui->listWidget2_1->currentItem();
    if(item->data(Qt::UserRole+1).toInt()==1)
    {
        // 新建的项
        cwd(CommandSocket,remote_pwd);
        mkd(CommandSocket,newName);

    } else {
        rename(CommandSocket, remote_lastItemName, newName);
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
//   远程地址栏
void MainWindow::on_lineEdit_2_returnPressed()
{
    string newAddress=ui->lineEdit_2->text().toStdString();

    //切换地址,cwd会产生比较多的返回信息，需要进行处理。
    cwd(CommandSocket,newAddress);
    char*  message=(char*)malloc(Dlength);
    memset(message,0,Dlength);
    recv(CommandSocket,message,Dlength,0);
    cout<<message;
    free(message);   //清理残余信息
    cout.flush();
    SOCKET datasock=pasv(CommandSocket);
    vector<File> list=ls(CommandSocket,datasock);
    setupList(ui->listWidget2_1,ui->listWidget2_2,ui->listWidget2_3,list);
    remote_pwd=pwd(CommandSocket);
}


