#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "utils/utils.h"
#include "utils/ErrorCode.h"
#include <QFile>
#include <QDir>
#include <QMainWindow>
#include <QtWidgets/QListWidget>
#include <utils/commands.h>
#include <QListWidgetItem>
#include <vector>
#include <QItemDelegate>
#include <QMenu>
#include "utils/resource.h"
#include "utils/utils.h"
#include "ftpsock/login.h"
#include "UI/login.h"
#include <QTableView>
#include <QScrollBar>
#include <QListWidget>
#include <QAction>
#include "ftpsock/qThread.h"
#include "FileMsg.h"
extern  vector<File> localFiles;
namespace Ui {
class MainWindow;
}
extern SOCKET CommandSocket;
extern ErrorCode  code;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    char* Password;
    char* Username;
    char* Ip;
    FileMsg msg;
    string local_pwd;
    string remote_pwd;
    string local_lastItemName;         // 此刻本地处理的item
    string remote_lastItemName;        // 此刻远程处理的item
    vector<File> localList;
    vector<File> remoteList;
    qThread* dataThread;               //数据线程
    int itemId=-1;                      // 下载项的唯一item值，每次打开的时候从零开始(每次在新建任务的开始+1)
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
     void setupList(QListWidget*w1,QListWidget*w2,QListWidget*w3,vector<File>list);
     void LocalRefresh();
    void RemoteRefresh();
    void addFinishItem(int row, int isUpload);

signals:
    void send_filemsg(FileMsg);
    void send_remote_path(string path);         //告知数据进程 远程目录的信号
    void send_local_path(string path);          //告知数据进程，本地当前目录
private slots:

     void on_listWidget1_1_itemSelectionChanged();

     void on_pushButton_upload_clicked();

     void on_pushButton_download_clicked();

     void on_pushButton_stop_clicked();

     void on_pushButton_cancel_clicked();

     void on_list2_3_scrollBar_value_changed(int action);
     void on_list1_3_scrollBar_value_changed(int action);

     void on_remote_list_edited(QWidget *editor);
     void on_lineEdit_returnPressed();

     void QFileInfoListToVector(QFileInfoList* qlist,vector<File>* list);

     void on_listWidget1_1_itemDoubleClicked(QListWidgetItem *item);

     void on_listWidget1_1_itemClicked(QListWidgetItem *item);

     void on_local_list_edited(QWidget *editor);

     void on_pushButton_clicked();         //本地刷新按钮

     void on_lineEdit_2_returnPressed();

     void on_pushButton_2_clicked();      //远程刷新按钮

     void on_listWidget2_1_itemSelectionChanged();

     void on_listWidget2_1_itemDoubleClicked(QListWidgetItem *item);

     void on_listWidget2_1_itemClicked(QListWidgetItem *item);

     void on_localMenu_addDir_triggered();

     void on_localMenu_delItem_triggered();

     void on_remoteMenu_addDir_triggered();

    void on_remoteMenu_delItem_triggered();

    void on_progressBar_valueChanged(int value,int id);

    void receiveStateChange(int id, int state, int isUpload);

    void on_finishOneTask(int id, int nextId, int isUpload);        //  后台完成一个项目的下载，本地移除列表的一个项目

    void receive_Dir_fileCount(int filecount,int id); // 接受文件夹的文件数目
public:
    Ui::MainWindow *ui;


};



#endif // MAINWINDOW_H
