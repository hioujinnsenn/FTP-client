#include <QtWidgets/QProgressBar>
#include <QtWidgets/QHBoxLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "UI/Concurrent/JhProgressBar.h"
#include "UI/Concurrent/JhButton.h"
extern  string username;
extern string password;
extern string ip;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setWindowIcon(QIcon("../UI/resoucre/icon/64/ftp.png"));
    setupList(ui->listWidget1_1,ui->listWidget1_2,ui->listWidget1_3,localFiles);
    qRegisterMetaType<FileMsg>("FileMsg");             // 注册FileMsg类为元信息
    qRegisterMetaType<string>("string");
    //赋值username和password
    this->Username=(char*)malloc(400);
    this->Password=(char*)malloc(400);
    this->Ip=(char*)malloc(400);
    memset(this->Username,0,400);
    memset(this->Password,0,400);
    memset(this->Ip,0,400);
    sprintf(this->Username,username.data());
    sprintf(this->Password,password.data());
    sprintf(this->Ip,ip.data());

    SOCKET sock=login(this->Username,this->Password,this->Ip);
    // ls接口设计的不够简洁，造成过量的冗余
    SOCKET datasock=pasv(sock);
    vector<File> serverList=ls(sock,datasock);
    setupList(ui->listWidget2_1,ui->listWidget2_2,ui->listWidget2_3,serverList);

    //移除多余的自动tab标签生成页
    ui->tabWidget_2->removeTab(1);
    ui->tabWidget->removeTab(1);

    // 增加四个主要工具按钮的icon路径
    QIcon upload("../UI/resoucre/icon/48/upload.png");
    ui->pushButton_upload->setIcon(upload);
    QIcon download("../UI/resoucre/icon/48/download.png");
    ui->pushButton_download->setIcon(download);
    QIcon stop("../UI/resoucre/icon/48/stop.png");
    ui->pushButton_stop->setIcon(stop);
    QIcon cancel("../UI/resoucre/icon/48/cancel.png");
    ui->pushButton_cancel->setIcon(cancel);
    // TODO 还应该对四个按钮的槽函数进行编写，在下面的函数里设置好了，但是目前是空的
    // TODO 注意的是对于stop按钮，点击后应变成continue按钮.continue还能切换回stop

    //      暂时不添加restart按钮

    // Important : 新增的自定义槽连接，用于同步三个列表。
    connect(ui->listWidget2_3->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(on_list2_3_scrollBar_value_changed(int)));
    connect(ui->listWidget1_3->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(on_list1_3_scrollBar_value_changed(int)));

    // 避免重复多次定义同一个槽和连接，会爆炸。
    QItemDelegate* delegate=new QItemDelegate(ui->listWidget1_1);
    //只需要将commitData信号牵引出来就可以了
    connect(delegate,SIGNAL(commitData(QWidget*)),this,SLOT(on_local_list_edited(QWidget*)));
    ui->listWidget1_1->setItemDelegate(delegate);

    // 远程端代理，处理编辑事件
    QItemDelegate* remoteDelegate=new QItemDelegate(ui->listWidget2_1);
    connect(remoteDelegate,SIGNAL(commitData(QWidget*)),this,SLOT(on_remote_list_edited(QWidget*)));
    ui->listWidget2_1->setItemDelegate(remoteDelegate);

    // 本地右键菜单设置
    QAction *addDir=new QAction("新建文件夹",ui->listWidget1_1);
    QAction *delItem=new QAction("删除",ui->listWidget1_1);
    connect(addDir,SIGNAL(triggered()),this,SLOT(on_localMenu_addDir_triggered()));
    connect(delItem,SIGNAL(triggered()),this,SLOT(on_localMenu_delItem_triggered()));
    ui->listWidget1_1->addAction(addDir);
    ui->listWidget1_1->addAction(delItem);

    // 远程右键菜单设置
    QAction *remoteAddDir=new QAction("新建文件夹",ui->listWidget2_1);
    QAction *remoteDelItem=new QAction("删除",ui->listWidget2_1);
    connect(remoteAddDir,SIGNAL(triggered()),this,SLOT(on_remoteMenu_addDir_triggered()));
    connect(remoteDelItem,SIGNAL(triggered()),this,SLOT(on_remoteMenu_delItem_triggered()));
    ui->listWidget2_1->addAction(remoteAddDir);
    ui->listWidget2_1->addAction(remoteDelItem);
    //退出释放sock
    SendCommand(sock,QUIT);

    //创建伴随存在的子线程
    this->dataThread=new qThread(this->Username, this->Password, this->Ip);
    connect(dataThread, SIGNAL(sendProgress(int,int)), this, SLOT(on_progressBar_valueChanged(int,int)));   //进度条数据绑定槽函数
    connect(dataThread, SIGNAL(finishOne(int,int)), this, SLOT(on_finishOneTask(int, int)));                //任务结束信号绑定界面槽函数
    connect(this,SIGNAL(send_filemsg(FileMsg)),dataThread,SLOT(receive_filemsg(FileMsg)));//传输文件信息和id
    // 通信通道--传输remote_path、local_path
    connect(this,SIGNAL(send_local_path(string)),dataThread,SLOT(receive_local_path(string)));
    connect(this,SIGNAL(send_remote_path(string)),dataThread,SLOT(receive_remote_path(string)));
    // 通信通道--传输文件夹内的文件数目
    connect(dataThread,SIGNAL(send_Dir_filecount(int ,int)),this,SLOT(receive_Dir_fileCount(int,int)));
    //应该还需要绑定其他的，比如暂停，比如终止
    this->dataThread->start();

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
             if(lists[i].name==".")
                continue;
              QListWidgetItem*i1=new QListWidgetItem(w1);
              QVariant path=QString(lists[i].path.data());
              i1->setData(Qt::UserRole,path);             //文件路径
              QVariant isNew=0;
              i1->setData(Qt::UserRole+1,isNew);          //是不是新建的，只有在右键新建的时候才使用
              QVariant isDir=1;                                //是一个文件
              i1->setData(Qt::UserRole+2,isDir);          //是文件还是目录
              QVariant size=qlonglong(lists[i].size);
              i1->setData(Qt::UserRole+3, size);
              QListWidgetItem*i2=new QListWidgetItem(w2);
              QListWidgetItem*i3=new QListWidgetItem(w3);
              //去掉当前目录的项
              i1->setText(lists[i].name.data());
              //icon是否生效需要进行检验
              QIcon  icon("../UI/resoucre/icon/48/dir.png");
              i1->setIcon(icon);
              i2->setText("");
              i3->setText("系统文件夹");

        } else{
            QListWidgetItem*i1=new QListWidgetItem(w1);
            QVariant path=QString(lists[i].path.data());
            i1->setData(Qt::UserRole,path);
            QVariant isNew=0;
            i1->setData(Qt::UserRole+1,isNew);
            QVariant isDir=0;
            i1->setData(Qt::UserRole+2,isDir);
            QVariant fsize=qlonglong(lists[i].size);
            i1->setData(Qt::UserRole+3, fsize);
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

void MainWindow::on_pushButton_upload_clicked()                                //点击上传
{
    SOCKET sock=login(this->Username,this->Password,this->Ip);
    QList<QListWidgetItem*> files= this->ui->listWidget1_1->selectedItems();
    vector<string> paths;
    vector<int> ids;

    for(int i=0; i<files.size(); i++){

        // 每次id累加器进行递加
        if(this->itemId==INT32_MAX)
            this->itemId=0;
        else
            this->itemId++;
        FileMsg msg;
        msg.UpOrDown=0;      //上传
        msg.status=0  ;      //没有被暂停的项目
        ids.push_back(this->itemId);
        msg.id=this->itemId;
        QString filePath=files.at(i)->data(Qt::UserRole).toString();
        paths.push_back(filePath.toStdString());                                 //得到string类型的vector，存储所有选中需要上传的文件或目录
        msg.filepath=filePath.toStdString();
        QListWidgetItem* i_name=new QListWidgetItem(ui->listWidget_name);        //一项文件的名字
        i_name->setText(files.at(i)->text());

        QListWidgetItem* i_status=new QListWidgetItem(ui->listWidget_status);    //文件状态（上传中、暂停）
        i_status->setText("上传中");

        QListWidgetItem* i_progress=new QListWidgetItem(ui->listWidget_progress);   //文件上传进度
        i_progress->setSizeHint(QSize(300,30));
        QVariant itemId=this->itemId;
        i_progress->setData(Qt::UserRole,itemId);
        QWidget* w=new QWidget(ui->listWidget_progress);
        w->setGeometry(0,30*i+10,300,30);
        QHBoxLayout* layout=new QHBoxLayout(w);
        layout->setMargin(0);                                                       //重点,清除留白
        JhProgressBar *progressBar=new JhProgressBar(w);                            //item内插入进度条
        progressBar->setId(this->itemId);
        progressBar->setFormat(QString("当前的进度为:%p%"));             //自定义文字，其中%p代表百分比，后一个%单纯是%而已
        progressBar->setAlignment(Qt::AlignHCenter);                                //水平方向上居中进度文字
        progressBar->setAlignment(Qt::AlignVCenter);                                //垂直方向上居中文字
        progressBar->setGeometry(QRect(0, 5, 220, 30));
        progressBar->setValue(0);
                                                                                    //        cout<<"当前在第几行进度："<<i_progress-><<endl;
        JhButton* pushButton_pause=new JhButton(w);                                 //item插入暂停/继续按钮
        pushButton_pause->setId(this->itemId);
        pushButton_pause->ButtonType=0;                                             //设置按钮类型
        pushButton_pause->setObjectName("pauseButton");
        pushButton_pause->setGeometry(QRect(230, 5, 30, 25));
        QIcon pause("../UI/resoucre/icon/48/stop.png");
        pushButton_pause->setIcon(pause);
        connect(pushButton_pause,SIGNAL(send_pause_id(int)),this->dataThread,SLOT(receive_pause_id(int)));  //接收暂停项目的id
        JhButton* pushButton_terminate=new JhButton(w);                             //item插入终止按钮
        pushButton_terminate->setId(this->itemId);
        pushButton_terminate->setObjectName("terminateButton");
        pushButton_terminate->setGeometry(QRect(270, 5, 30, 25));
        QIcon terminate("../UI/resoucre/icon/48/cancel.png");
        pushButton_terminate->setIcon(terminate);
        layout->addWidget(progressBar);
        layout->addWidget(pushButton_pause);
        layout->addWidget(pushButton_terminate);
        w->setLayout(layout);
        ui->listWidget_progress->setItemWidget(i_progress, w);

        QListWidgetItem* i_size=new QListWidgetItem(ui->listWidget_size);                //文件大小
        i_size->setText(files[i]->data(Qt::UserRole+3).toString()+"B");

        QListWidgetItem* i_localPath=new QListWidgetItem(ui->listWidget_localPath);      //文件本地路径
        i_localPath->setText(filePath);

        QListWidgetItem* i_remotePath=new QListWidgetItem(ui->listWidget_remotePath);   //文件上传到服务器路径
        i_remotePath->setText(QString::fromStdString(pwd(sock)));
        //----------------------------------
            emit send_filemsg(msg);            //传送文件信息和id到数据线程端
        //----------------------------------
    }
}

void MainWindow::on_pushButton_download_clicked()   //下载按钮
{
    //默认就是下载到本地路径的现在的位置上
    SOCKET sock=login(this->Username,this->Password,this->Ip);
    vector<string> paths;
    vector<int> ids;
   QList<QListWidgetItem*>  items=ui->listWidget2_1->selectedItems();
   for(int i=0;i<items.size();i++)
   {
       this->itemId++;
       QListWidgetItem* item=items[i];
       FileMsg msg;
       msg.UpOrDown=1;  //下载
       msg.status=0;    //没有被设置暂停
       ids.push_back(this->itemId);
       msg.id=this->itemId;
       QString filePath=items.at(i)->data(Qt::UserRole).toString();
       paths.push_back(filePath.toStdString());                                 //得到string类型的vector，存储所有选中需要上传的文件或目录
       msg.filepath=filePath.toStdString();
       QListWidgetItem* i_name=new QListWidgetItem(ui->listWidget_name);        //一项文件的名字
       i_name->setText(items.at(i)->text());

       QListWidgetItem* i_status=new QListWidgetItem(ui->listWidget_status);    //文件状态（上传中、暂停）
       i_status->setText("上传中");

       QListWidgetItem* i_progress=new QListWidgetItem(ui->listWidget_progress);   //文件上传进度
       i_progress->setSizeHint(QSize(300,30));
       QVariant itemId=this->itemId;
       i_progress->setData(Qt::UserRole,itemId);
       QWidget* w=new QWidget(ui->listWidget_progress);
       w->setGeometry(0,30*i+10,300,30);
       QHBoxLayout* layout=new QHBoxLayout(w);
       layout->setMargin(0);                                                       //重点,清除留白
       JhProgressBar *progressBar=new JhProgressBar(w);                            //item内插入进度条
       progressBar->setId(this->itemId);
       progressBar->setFormat(QString("当前的进度为:%p%"));             //自定义文字，其中%p代表百分比，后一个%单纯是%而已
       progressBar->setAlignment(Qt::AlignHCenter);                                //水平方向上居中进度文字
       progressBar->setAlignment(Qt::AlignVCenter);                                //垂直方向上居中文字
       progressBar->setGeometry(QRect(0, 5, 220, 30));
       progressBar->setValue(0);
       JhButton* pushButton_pause=new JhButton(w);   //item插入暂停/继续按钮
       pushButton_pause->ButtonType=0;                    // 设置按钮的功能类型
       pushButton_pause->setId(this->itemId);
       pushButton_pause->setObjectName("pauseButton");
       pushButton_pause->setGeometry(QRect(230, 5, 30, 25));
       // 连接暂停按钮和数据线程
       // TODO 在删除按钮时，需要disconnet，避免对线程造成影响
       connect(pushButton_pause,SIGNAL(send_pause_id(int)),this->dataThread,SLOT(receive_pause_id(int)));
       QIcon pause("../UI/resoucre/icon/48/stop.png");
       pushButton_pause->setIcon(pause);
       if (i) pushButton_pause->hide();                                            //由于一次只跑一个，所以初始化只显示第一个暂停按钮，一次只显示当前正在跑的项目的暂停键
       JhButton* pushButton_terminate=new JhButton(w);                             //item插入终止按钮
       pushButton_terminate->setId(this->itemId);
       pushButton_terminate->setObjectName("terminateButton");
       pushButton_terminate->setGeometry(QRect(270, 5, 30, 25));

       QIcon terminate("../UI/resoucre/icon/48/cancel.png");
       pushButton_terminate->setIcon(terminate);
       layout->addWidget(progressBar);
       layout->addWidget(pushButton_pause);
       layout->addWidget(pushButton_terminate);
       w->setLayout(layout);
       ui->listWidget_progress->setItemWidget(i_progress, w);

       QListWidgetItem* i_size=new QListWidgetItem(ui->listWidget_size);                //文件大小
       i_size->setText(items[i]->data(Qt::UserRole+3).toString()+"B");

       QListWidgetItem* i_localPath=new QListWidgetItem(ui->listWidget_localPath);      //文件本地路径
       i_localPath->setText(filePath);

       QListWidgetItem* i_remotePath=new QListWidgetItem(ui->listWidget_remotePath);   //文件上传到服务器路径
       i_remotePath->setText(QString::fromStdString(pwd(sock)));
       //----------------------------------
         emit send_filemsg(msg);            //传送文件信息和id到数据线程端
       //----------------------------------
   }
}

void MainWindow::on_pushButton_stop_clicked()   //四个按钮里的暂停按钮
{

}

void MainWindow::on_pushButton_cancel_clicked()  //四个按钮里的取消按钮
{

}


// 文件信息列表转为vector
void MainWindow::QFileInfoListToVector(QFileInfoList *qlist, vector<File> *list) {

    list->clear();    //清空原有的list
    for(int i=0;i<qlist->size();i++)
    {
        File file;
        if(qlist->operator[](i).isFile())
        {
            file.size=qlist->operator[](i).size();
            file.name=qlist->operator[](i).fileName().toStdString();
            file.path=qlist->operator[](i).absoluteFilePath().toStdString();
            file.type=2; //普通文件
        }
        else{
            file.path=qlist->operator[](i).absoluteFilePath().toStdString();
            file.name=qlist->operator[](i).fileName().toStdString();
            file.type=1; //目录文件
        }
        list->push_back(file);
    }
}

void MainWindow::on_progressBar_valueChanged(int value,int id)     //修改进度条数据
{
    int count=this->ui->listWidget_progress->count();
    for (int i=0;i<count;i++)
    {
        QListWidgetItem* item=this->ui->listWidget_progress->item(i);

        if(item->data(Qt::UserRole).toInt()==id)                // 通过匹配id值找到对应的进度条进行修改
        {
            JhProgressBar* progressBar=ui->listWidget_progress->itemWidget(item)->findChild<JhProgressBar*>();
            progressBar->setValue(value);
            return ;
        }
    }

}

void MainWindow::on_pushButton_pause_clicked(int id)    //做了绑定？
{
    int count=this->ui->listWidget_progress->count();
    for(int i=0;i<count; i++){
        QListWidgetItem* item=ui->listWidget_progress->item(i);
        if(item->data(Qt::UserRole).toInt()==id){
            JhButton* pauseButton=ui->listWidget_progress->itemWidget(item)->findChild<JhButton*>("pauseButton");
            QIcon icon;
            if(pauseButton->state==0)   //当前任务正在上传
                icon=QIcon("../UI/resoucre/icon/48/continue.png");
            else icon=QIcon("../UI/resoucre/icon/48/stop.png");
            pauseButton->setIcon(icon); //改变按钮图标
            cout<<"改变暂停键图标！"<<endl;
            pauseButton->state^=1;
            return;
        }
    }
}

void MainWindow::on_pushButton_terminate_clicked(int id)
{

}



void MainWindow::on_finishOneTask(int id, int nextId)
{
    cout<<"任务完成"<<id<<"，下一个任务："<<nextId<<endl;
    int count=ui->listWidget_progress->count();
    int i;
    for(i=0;i<count;i++){
        //删除本任务
        QListWidgetItem* item=ui->listWidget_progress->item(i);
        if(item->data(Qt::UserRole).toInt()==id){
            ui->listWidget_progress->removeItemWidget(item);
            ui->listWidget_progress->takeItem(i);
//            delete item;
            ui->listWidget_name->takeItem(i);
            ui->listWidget_status->takeItem(i);
            ui->listWidget_size->takeItem(i);
            ui->listWidget_localPath->takeItem(i);
            ui->listWidget_remotePath->takeItem(i);
            //下一项任务的暂停按钮取消隐藏
            if(nextId!=-1) {
                QListWidgetItem *nextItem = ui->listWidget_progress->item(nextId);
                JhButton *pauseButton = ui->listWidget_progress->itemWidget(nextItem)->findChild<JhButton *>("pauseButton");
                pauseButton->show();
            }
//            RemoteRefresh();
            break;
        }
    }

}


     //  接受文件夹的文件数目
void MainWindow::receive_Dir_fileCount(int filecount, int id) {
    for (int i=0;i<ui->listWidget_progress->count();i++)
    {
        QListWidgetItem* item=ui->listWidget_progress->item(i);
        if(item->data(Qt::UserRole).toInt()==id)
        {
            stringstream  textStream;
            textStream<<filecount;
            textStream<<"个";
            // 设置文件数目
            string text=textStream.str();
            QListWidgetItem*  sizeItem=ui->listWidget_size->item(i);
            sizeItem->setText(text.data());
        }
    }
}
