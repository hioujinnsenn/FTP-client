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
    //页面内的单独操作要做一次sock创建和退出
    SOCKET sock=login(this->Username,this->Password,this->Ip);
    cwd(sock,this->remote_pwd);
    SOCKET datasock=pasv(sock);
    vector<File> serverList=ls(sock,datasock);
    File parent;
    parent.type=1;
    parent.name="..";
    parent.path="..";
    serverList.insert(serverList.begin(),parent);
    setupList(ui->listWidget2_1,ui->listWidget2_2,ui->listWidget2_3,serverList);
    SendCommand(sock,QUIT);
}


void MainWindow::on_listWidget2_1_itemSelectionChanged()
{

}

//  远程窗口的列表项双击
void MainWindow::on_listWidget2_1_itemDoubleClicked(QListWidgetItem *item) {
    SOCKET sock=login(this->Username,this->Password,this->Ip);
    string name=item->text().toStdString();
    cwd(sock,this->remote_pwd);            //先同步远程 本地的目录层次
    string returnInfo =cwd(sock,name);           //cwd 切换目录
    SOCKET datasock=pasv(sock);
    vector<File> serverList=ls(sock,datasock);
    File parent;
    parent.type=1;
    parent.name="..";
    parent.path="..";
    serverList.insert(serverList.begin(),parent);
    setupList(ui->listWidget2_1,ui->listWidget2_2,ui->listWidget2_3,serverList);
    this->remote_pwd=pwd(sock);   //更新远程目录
    SendCommand(sock,QUIT);
    emit send_remote_path(this->remote_pwd); //同步更新子线程中的数据，不是同步的，没有共享内存快，可以用共享内存

}


void MainWindow::on_listWidget2_1_itemClicked(QListWidgetItem *item)
{
    item->setFlags(item->flags()|Qt::ItemIsEditable);
    remote_lastItemName=item->text().toStdString();      //远程当前选中名称
}

//远程端的item修改名称
void MainWindow::on_remote_list_edited(QWidget *editor) {

    SOCKET sock=login(this->Username,this->Password,this->Ip);
    string newName=((QLineEdit*)editor)->text().toStdString();
    QListWidgetItem * item=ui->listWidget2_1->currentItem();
    if(item->data(Qt::UserRole+1).toInt()==1)
    {
        // 新建的项
        cwd(sock,remote_pwd);
        mkd(sock,newName);

    } else {
        cwd(sock,remote_pwd);
        rename(sock, remote_lastItemName, newName);
    }

    SOCKET datasock=pasv(sock);
    vector<File> serverList=ls(sock,datasock);
    File parent;
    parent.type=1;
    parent.name="..";
    parent.path="..";
    serverList.insert(serverList.begin(),parent);
    setupList(ui->listWidget2_1,ui->listWidget2_2,ui->listWidget2_3,serverList);
    SendCommand(sock,QUIT);

}

//   远程地址栏
void MainWindow::on_lineEdit_2_returnPressed()
{
    SOCKET sock=login(this->Username,this->Password,this->Ip);
    string newAddress=ui->lineEdit_2->text().toStdString();

    cwd(sock,newAddress);
    cout.flush();
    SOCKET datasock=pasv(sock);
    vector<File> list=ls(sock,datasock);
    setupList(ui->listWidget2_1,ui->listWidget2_2,ui->listWidget2_3,list);
    this->remote_pwd=pwd(sock);
    emit send_remote_path(this->remote_pwd); //同步更新子线程中的数据，不是同步的，没有共享内存快，可以用共享内存
}

//封装远端刷新
void MainWindow::RemoteRefresh()
{
    SOCKET sock=login(this->Username,this->Password,this->Ip);
    cwd(sock,this->remote_pwd);                         //切换至与本地显示的同步的位置
    SOCKET datasock=pasv(sock);
    vector<File> serverList=ls(sock,datasock);
    File parent;
    parent.type=1;
    parent.name="..";
    parent.path="..";
    serverList.insert(serverList.begin(),parent);
    setupList(ui->listWidget2_1,ui->listWidget2_2,ui->listWidget2_3,serverList);
    SendCommand(sock,QUIT);
}
