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
#include "ftpsock/upload_qThread.h"
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
    string local_pwd;
    string remote_pwd;
    string local_lastItemName;
    string remote_lastItemName;
    vector<File> localList;
    vector<File> remoteList;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
     void setupList(QListWidget*w1,QListWidget*w2,QListWidget*w3,vector<File>list);
     void LocalRefresh();
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

     void on_pushButton_clicked();

     void on_lineEdit_2_returnPressed();

     void on_pushButton_2_clicked();

     void on_listWidget2_1_itemSelectionChanged();

     void on_listWidget2_1_itemDoubleClicked(QListWidgetItem *item);

     void on_listWidget2_1_itemClicked(QListWidgetItem *item);

     void on_localMenu_addDir_triggered();

     void on_localMenu_delItem_triggered();

     void on_remoteMenu_addDir_triggered();

    void on_remoteMenu_delItem_triggered();



    void on_progressBar_valueChanged(int value);

    void on_pushButton_pause_clicked();

    void on_pushButton_terminate_clicked();

    void on_listWidget_progress_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
