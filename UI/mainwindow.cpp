#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils/utils.h"
#include "ftpsock/login.h"
#include "UI/login.h"
#include <QTableView>
#include <QScrollBar>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupList(ui->listWidget1_1,ui->listWidget1_2,ui->listWidget1_3,localFiles);

    //ls接口设计的不够简洁，造成过量的冗余
    SOCKET datasock=pasv(CommandSocket);
    vector<File> serverList=ls(CommandSocket,datasock);
    setupList(ui->listWidget2_1,ui->listWidget2_2,ui->listWidget2_3,serverList);

    //移除多余的自动tab标签生成页
    ui->tabWidget_2->removeTab(1);
    ui->tabWidget->removeTab(1);

    //增加四个主要工具按钮的icon路径
    QIcon upload("../UI/resoucre/icon/48/upload.png");
    ui->pushButton_upload->setIcon(upload);
    QIcon download("../UI/resoucre/icon/48/download.png");
    ui->pushButton_download->setIcon(download);
    QIcon stop("../UI/resoucre/icon/48/stop.png");
    ui->pushButton_stop->setIcon(stop);
    QIcon cancel("../UI/resoucre/icon/48/cancel.png");
    ui->pushButton_cancel->setIcon(cancel);
    // TODO 还应该对四个按钮的槽函数进行编写，在下面的函数里设置好了，但是目前是空的
    // TODO 注意的是对于stop按钮，点击后应变成continue按钮
    //      暂时不添加restart按钮

    // Important : 新增的自定义槽连接，用于同步三个列表。
    connect(ui->listWidget2_3->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(on_list2_3_scrollBar_value_changed(int)));
    connect(ui->listWidget1_3->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(on_list1_3_scrollBar_value_changed(int)));

}

//   两个辅助函数，应设计进类中
string getFileType(string name) {
    int i = name.find_last_of(".");
    return name.substr(i + 1);
}

//需检查是否生效
void setFileIcon(QListWidgetItem* item,string type)
{
    string iconPath="../UI/resoucre/icon/48/";
    iconPath.append(type);
    iconPath.append("-ext.png");
    QFile file(iconPath.data());
    if(file.exists())
    {
        //找到匹配项目
        QIcon icon(iconPath.data());
        item->setIcon(icon);
    }
    else{
        //未添加的匹配类型
        QIcon  icon("../UI/resoucre/icon/48/unknown-ext.png");
        item->setIcon(icon);
    }
}
//
MainWindow::~MainWindow()
{
    delete ui;
}

//设置文件列表
void MainWindow::setupList(QListWidget* w1,QListWidget *w2,QListWidget*w3,vector<File> lists)
{
    //清空旧有数据
    w1->clear();
    w2->clear();
    w3->clear();
    for(int i=0;i<lists.size();i++)
    {
        if(lists[i].type==1)
        {
            //目录文件
            //以下代码错误的原因在于是栈内的局部变量，被销毁
//            QListWidgetItem item1,item2,item3;
//            item1.setText(lists[i].name.data());
//            item3.setText("系统文件夹");
//            item2.setText("");
//            QIcon icon("UI/resoucre/icon/48/dir.png");
//            item1.setIcon(icon);
//            w1->addItem(&item1);
//            w2->addItem(&item2);
//            w3->addItem(&item3);
              QListWidgetItem*i1=new QListWidgetItem(w1);

              QListWidgetItem*i2=new QListWidgetItem(w2);
              QListWidgetItem*i3=new QListWidgetItem(w3);
              i1->setText(lists[i].name.data());
              //icon是否生效需要进行检验
              QIcon  icon("../UI/resoucre/icon/48/dir.png");
              i1->setIcon(icon);
              i2->setText("");
              i3->setText("系统文件夹");

        } else{
            QListWidgetItem*i1=new QListWidgetItem(w1);
            QListWidgetItem*i2=new QListWidgetItem(w2);
            QListWidgetItem*i3=new QListWidgetItem(w3);
            i1->setText(lists[i].name.data());
            string type=getFileType(lists[i].name);
            setFileIcon(i1,type);
            i3->setText(type.data());
            string size="";
            char* m=(char*)malloc(400);
            memset(m,0,400);
            sprintf(m,"%ld",lists[i].size);
            size.append(m);
            size.append("B");
            i2->setText(size.data());
            free(m);



        }
    }


}
// NOTE :list2_3是服务器端widget的第三个，也就是最后一个
void MainWindow::on_list2_3_scrollBar_value_changed(int action) {
     ui->listWidget2_1->verticalScrollBar()->setValue(action);
     ui->listWidget2_2->verticalScrollBar()->setValue(action);
}

//本地端
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

void MainWindow::on_pushButton_upload_clicked()
{

}

void MainWindow::on_pushButton_download_clicked()
{

}

void MainWindow::on_pushButton_stop_clicked()
{

}

void MainWindow::on_pushButton_cancel_clicked()
{

}


