#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "utils/utils.h"
#include "utils/ErrorCode.h"
#include <QFile>
#include <QMainWindow>
#include <QtWidgets/QListWidget>
#include <utils/commands.h>
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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
     void setupList(QListWidget*w1,QListWidget*w2,QListWidget*w3,vector<File>list);
private slots:
     void on_listWidget1_1_itemSelectionChanged();

     void on_pushButton_upload_clicked();

     void on_pushButton_download_clicked();

     void on_pushButton_stop_clicked();

     void on_pushButton_cancel_clicked();

     void on_list2_3_scrollBar_value_changed(int action);
     void on_list1_3_scrollBar_value_changed(int action);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
