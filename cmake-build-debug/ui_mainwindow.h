/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QListWidget *listWidget1_1;
    QListWidget *listWidget1_2;
    QListWidget *listWidget1_3;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QWidget *tab_2;
    QTabWidget *tabWidget_2;
    QWidget *tab_3;
    QListWidget *listWidget2_2;
    QListWidget *listWidget2_3;
    QListWidget *listWidget2_1;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_2;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QWidget *tab_4;
    QPushButton *pushButton_upload;
    QPushButton *pushButton_download;
    QPushButton *pushButton_stop;
    QPushButton *pushButton_cancel;
    QTabWidget *tabWidget_3;
    QWidget *tab_transporting;
    QListWidget *listWidget_name;
    QLabel *label_name;
    QLabel *label_status;
    QListWidget *listWidget_status;
    QLabel *label_process;
    QLabel *label_size;
    QListWidget *listWidget_size;
    QLabel *label_localPath;
    QLabel *label_remotePath;
    QListWidget *listWidget_localPath;
    QListWidget *listWidget_remotePath;
    QListWidget *listWidget_progress;
    QWidget *tab_finnished;
    QLabel *label_fname;
    QLabel *label_fstatus;
    QLabel *label_flocalPath;
    QLabel *label_fremotePath;
    QLabel *label_fsize;
    QListWidget *listWidget_fname;
    QListWidget *listWidget_fstatus;
    QListWidget *listWidget_fsize;
    QListWidget *listWidget_floaclPath;
    QListWidget *listWidget_fremotePath;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1650, 1080);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 800, 790));
        tabWidget->setTabsClosable(false);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tab->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        listWidget1_1 = new QListWidget(tab);
        listWidget1_1->setObjectName(QString::fromUtf8("listWidget1_1"));
        listWidget1_1->setGeometry(QRect(0, 105, 400, 660));
        listWidget1_1->setContextMenuPolicy(Qt::ActionsContextMenu);
        listWidget1_1->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border:5px\n"
""));
        listWidget1_1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget1_1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget1_1->setEditTriggers(QAbstractItemView::SelectedClicked);
        listWidget1_1->setSelectionMode(QAbstractItemView::ExtendedSelection);
        listWidget1_1->setGridSize(QSize(400, 40));
        listWidget1_2 = new QListWidget(tab);
        listWidget1_2->setObjectName(QString::fromUtf8("listWidget1_2"));
        listWidget1_2->setGeometry(QRect(400, 105, 200, 660));
        listWidget1_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border:5px;"));
        listWidget1_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget1_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget1_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidget1_2->setGridSize(QSize(200, 40));
        listWidget1_3 = new QListWidget(tab);
        listWidget1_3->setObjectName(QString::fromUtf8("listWidget1_3"));
        listWidget1_3->setGeometry(QRect(600, 105, 190, 660));
        listWidget1_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border:5px;"));
        listWidget1_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget1_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidget1_3->setGridSize(QSize(200, 40));
        listWidget1_3->setSortingEnabled(false);
        lineEdit = new QLineEdit(tab);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(50, 10, 471, 51));
        lineEdit->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"border:1px solid;\n"
"border-color: rgb(31, 0, 47);"));
        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(540, 10, 81, 51));
        pushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-radius:10px;\n"
"border:1px solid"));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 65, 400, 40));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(400, 65, 200, 40));
        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(600, 65, 200, 40));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tab_2->setEnabled(false);
        tabWidget->addTab(tab_2, QString());
        tabWidget_2 = new QTabWidget(centralWidget);
        tabWidget_2->setObjectName(QString::fromUtf8("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(850, 0, 800, 790));
        tabWidget_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        tabWidget_2->setTabsClosable(false);
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        listWidget2_2 = new QListWidget(tab_3);
        listWidget2_2->setObjectName(QString::fromUtf8("listWidget2_2"));
        listWidget2_2->setGeometry(QRect(400, 105, 200, 660));
        listWidget2_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border:5px;"));
        listWidget2_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget2_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget2_2->setGridSize(QSize(200, 40));
        listWidget2_3 = new QListWidget(tab_3);
        listWidget2_3->setObjectName(QString::fromUtf8("listWidget2_3"));
        listWidget2_3->setGeometry(QRect(600, 105, 190, 660));
        listWidget2_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border:5px;"));
        listWidget2_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget2_3->setGridSize(QSize(200, 40));
        listWidget2_1 = new QListWidget(tab_3);
        listWidget2_1->setObjectName(QString::fromUtf8("listWidget2_1"));
        listWidget2_1->setGeometry(QRect(0, 105, 400, 660));
        listWidget2_1->setContextMenuPolicy(Qt::ActionsContextMenu);
        listWidget2_1->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border:5px;"));
        listWidget2_1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget2_1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget2_1->setEditTriggers(QAbstractItemView::SelectedClicked);
        listWidget2_1->setSelectionMode(QAbstractItemView::ExtendedSelection);
        listWidget2_1->setGridSize(QSize(400, 40));
        lineEdit_2 = new QLineEdit(tab_3);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(50, 10, 471, 51));
        lineEdit_2->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"border:1px solid;\n"
"border-color: rgb(31, 0, 47);"));
        pushButton_2 = new QPushButton(tab_3);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(540, 10, 81, 51));
        pushButton_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-radius:10px;\n"
"border:1px solid"));
        label_4 = new QLabel(tab_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(0, 65, 400, 40));
        label_5 = new QLabel(tab_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(400, 65, 200, 40));
        label_6 = new QLabel(tab_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(600, 65, 200, 40));
        tabWidget_2->addTab(tab_3, QString());
        listWidget2_2->raise();
        listWidget2_3->raise();
        listWidget2_1->raise();
        pushButton_2->raise();
        lineEdit_2->raise();
        label_4->raise();
        label_5->raise();
        label_6->raise();
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        tab_4->setEnabled(false);
        tabWidget_2->addTab(tab_4, QString());
        pushButton_upload = new QPushButton(centralWidget);
        pushButton_upload->setObjectName(QString::fromUtf8("pushButton_upload"));
        pushButton_upload->setGeometry(QRect(800, 70, 50, 50));
        pushButton_upload->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-radius:3px;"));
        pushButton_download = new QPushButton(centralWidget);
        pushButton_download->setObjectName(QString::fromUtf8("pushButton_download"));
        pushButton_download->setGeometry(QRect(800, 150, 50, 50));
        pushButton_download->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-radius:3px;"));
        pushButton_stop = new QPushButton(centralWidget);
        pushButton_stop->setObjectName(QString::fromUtf8("pushButton_stop"));
        pushButton_stop->setGeometry(QRect(800, 250, 50, 50));
        pushButton_stop->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-radius:3px;"));
        pushButton_cancel = new QPushButton(centralWidget);
        pushButton_cancel->setObjectName(QString::fromUtf8("pushButton_cancel"));
        pushButton_cancel->setGeometry(QRect(800, 350, 50, 50));
        pushButton_cancel->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-radius:3px;"));
        tabWidget_3 = new QTabWidget(centralWidget);
        tabWidget_3->setObjectName(QString::fromUtf8("tabWidget_3"));
        tabWidget_3->setGeometry(QRect(0, 790, 1650, 241));
        tab_transporting = new QWidget();
        tab_transporting->setObjectName(QString::fromUtf8("tab_transporting"));
        listWidget_name = new QListWidget(tab_transporting);
        listWidget_name->setObjectName(QString::fromUtf8("listWidget_name"));
        listWidget_name->setGeometry(QRect(0, 31, 361, 161));
        listWidget_name->setStyleSheet(QString::fromUtf8("border:0px;"));
        listWidget_name->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget_name->setGridSize(QSize(361, 30));
        label_name = new QLabel(tab_transporting);
        label_name->setObjectName(QString::fromUtf8("label_name"));
        label_name->setGeometry(QRect(0, 0, 361, 31));
        label_name->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 255, 255);"));
        label_status = new QLabel(tab_transporting);
        label_status->setObjectName(QString::fromUtf8("label_status"));
        label_status->setGeometry(QRect(361, 0, 100, 32));
        label_status->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 255, 255);"));
        listWidget_status = new QListWidget(tab_transporting);
        listWidget_status->setObjectName(QString::fromUtf8("listWidget_status"));
        listWidget_status->setGeometry(QRect(360, 31, 100, 161));
        listWidget_status->setStyleSheet(QString::fromUtf8("border:0px;"));
        listWidget_status->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget_status->setGridSize(QSize(100, 30));
        label_process = new QLabel(tab_transporting);
        label_process->setObjectName(QString::fromUtf8("label_process"));
        label_process->setGeometry(QRect(461, 0, 300, 31));
        label_process->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 255, 255);"));
        label_size = new QLabel(tab_transporting);
        label_size->setObjectName(QString::fromUtf8("label_size"));
        label_size->setGeometry(QRect(760, 0, 170, 31));
        label_size->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 255, 255);"));
        listWidget_size = new QListWidget(tab_transporting);
        listWidget_size->setObjectName(QString::fromUtf8("listWidget_size"));
        listWidget_size->setGeometry(QRect(760, 30, 170, 161));
        listWidget_size->setStyleSheet(QString::fromUtf8("border:0px;"));
        listWidget_size->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget_size->setGridSize(QSize(200, 30));
        label_localPath = new QLabel(tab_transporting);
        label_localPath->setObjectName(QString::fromUtf8("label_localPath"));
        label_localPath->setGeometry(QRect(930, 0, 361, 31));
        label_localPath->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 255, 255);"));
        label_remotePath = new QLabel(tab_transporting);
        label_remotePath->setObjectName(QString::fromUtf8("label_remotePath"));
        label_remotePath->setGeometry(QRect(1290, 0, 361, 31));
        label_remotePath->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 255, 255);"));
        listWidget_localPath = new QListWidget(tab_transporting);
        listWidget_localPath->setObjectName(QString::fromUtf8("listWidget_localPath"));
        listWidget_localPath->setGeometry(QRect(930, 30, 361, 161));
        listWidget_localPath->setStyleSheet(QString::fromUtf8("border:0px;"));
        listWidget_localPath->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget_localPath->setGridSize(QSize(361, 30));
        listWidget_remotePath = new QListWidget(tab_transporting);
        listWidget_remotePath->setObjectName(QString::fromUtf8("listWidget_remotePath"));
        listWidget_remotePath->setGeometry(QRect(1290, 30, 361, 161));
        listWidget_remotePath->setStyleSheet(QString::fromUtf8("border:0px;"));
        listWidget_remotePath->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget_remotePath->setGridSize(QSize(0, 30));
        listWidget_progress = new QListWidget(tab_transporting);
        listWidget_progress->setObjectName(QString::fromUtf8("listWidget_progress"));
        listWidget_progress->setGeometry(QRect(460, 31, 300, 161));
        listWidget_progress->setStyleSheet(QString::fromUtf8("border-radius:0px"));
        listWidget_progress->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget_progress->setGridSize(QSize(300, 30));
        tabWidget_3->addTab(tab_transporting, QString());
        listWidget_progress->raise();
        listWidget_name->raise();
        label_name->raise();
        label_status->raise();
        listWidget_status->raise();
        label_process->raise();
        label_size->raise();
        listWidget_size->raise();
        label_localPath->raise();
        label_remotePath->raise();
        listWidget_localPath->raise();
        listWidget_remotePath->raise();
        tab_finnished = new QWidget();
        tab_finnished->setObjectName(QString::fromUtf8("tab_finnished"));
        label_fname = new QLabel(tab_finnished);
        label_fname->setObjectName(QString::fromUtf8("label_fname"));
        label_fname->setGeometry(QRect(0, 0, 361, 31));
        label_fname->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 255, 255);"));
        label_fstatus = new QLabel(tab_finnished);
        label_fstatus->setObjectName(QString::fromUtf8("label_fstatus"));
        label_fstatus->setGeometry(QRect(360, 0, 100, 31));
        label_fstatus->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 255, 255);"));
        label_flocalPath = new QLabel(tab_finnished);
        label_flocalPath->setObjectName(QString::fromUtf8("label_flocalPath"));
        label_flocalPath->setGeometry(QRect(610, 0, 511, 31));
        label_flocalPath->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 255, 255);"));
        label_fremotePath = new QLabel(tab_finnished);
        label_fremotePath->setObjectName(QString::fromUtf8("label_fremotePath"));
        label_fremotePath->setGeometry(QRect(1122, 0, 531, 31));
        label_fremotePath->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 255, 255);"));
        label_fsize = new QLabel(tab_finnished);
        label_fsize->setObjectName(QString::fromUtf8("label_fsize"));
        label_fsize->setGeometry(QRect(460, 0, 151, 31));
        label_fsize->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 255, 255);"));
        listWidget_fname = new QListWidget(tab_finnished);
        listWidget_fname->setObjectName(QString::fromUtf8("listWidget_fname"));
        listWidget_fname->setGeometry(QRect(0, 31, 361, 161));
        listWidget_fname->setStyleSheet(QString::fromUtf8("border:0px;"));
        listWidget_fname->setGridSize(QSize(0, 30));
        listWidget_fstatus = new QListWidget(tab_finnished);
        listWidget_fstatus->setObjectName(QString::fromUtf8("listWidget_fstatus"));
        listWidget_fstatus->setGeometry(QRect(360, 31, 100, 161));
        listWidget_fstatus->setStyleSheet(QString::fromUtf8("border:0px;"));
        listWidget_fstatus->setGridSize(QSize(0, 30));
        listWidget_fsize = new QListWidget(tab_finnished);
        listWidget_fsize->setObjectName(QString::fromUtf8("listWidget_fsize"));
        listWidget_fsize->setGeometry(QRect(460, 31, 151, 161));
        listWidget_fsize->setStyleSheet(QString::fromUtf8("border:0px;"));
        listWidget_fsize->setGridSize(QSize(0, 30));
        listWidget_floaclPath = new QListWidget(tab_finnished);
        listWidget_floaclPath->setObjectName(QString::fromUtf8("listWidget_floaclPath"));
        listWidget_floaclPath->setGeometry(QRect(611, 31, 511, 161));
        listWidget_floaclPath->setStyleSheet(QString::fromUtf8("border:0px;"));
        listWidget_floaclPath->setGridSize(QSize(0, 30));
        listWidget_fremotePath = new QListWidget(tab_finnished);
        listWidget_fremotePath->setObjectName(QString::fromUtf8("listWidget_fremotePath"));
        listWidget_fremotePath->setGeometry(QRect(1130, 40, 511, 161));
        listWidget_fremotePath->setStyleSheet(QString::fromUtf8("border:0px;"));
        listWidget_fremotePath->setGridSize(QSize(0, 30));
        tabWidget_3->addTab(tab_finnished, QString());
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1650, 17));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(0);
        tabWidget_3->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "FTP", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\276\223\345\205\245\345\234\260\345\235\200", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "\345\210\267\346\226\260", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266\345\220\215", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\345\244\247\345\260\217", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\347\261\273\345\236\213", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "\346\241\214\351\235\242", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\276\223\345\205\245\345\234\260\345\235\200", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "\345\210\267\346\226\260", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266\345\220\215", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\345\244\247\345\260\217", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\347\261\273\345\236\213", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_3), QCoreApplication::translate("MainWindow", "\350\277\234\347\250\213\346\234\215\345\212\241\345\231\250", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_4), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
#if QT_CONFIG(whatsthis)
        pushButton_upload->setWhatsThis(QCoreApplication::translate("MainWindow", "\344\270\212\344\274\240\346\214\211\351\222\256", nullptr));
#endif // QT_CONFIG(whatsthis)
        pushButton_upload->setText(QString());
#if QT_CONFIG(whatsthis)
        pushButton_download->setWhatsThis(QCoreApplication::translate("MainWindow", "\344\270\213\350\275\275\346\214\211\351\222\256", nullptr));
#endif // QT_CONFIG(whatsthis)
        pushButton_download->setText(QString());
#if QT_CONFIG(whatsthis)
        pushButton_stop->setWhatsThis(QCoreApplication::translate("MainWindow", "\346\232\202\345\201\234\346\214\211\351\222\256", nullptr));
#endif // QT_CONFIG(whatsthis)
        pushButton_stop->setText(QString());
#if QT_CONFIG(whatsthis)
        pushButton_cancel->setWhatsThis(QCoreApplication::translate("MainWindow", "\345\217\226\346\266\210\346\214\211\351\222\256", nullptr));
#endif // QT_CONFIG(whatsthis)
        pushButton_cancel->setText(QString());
        label_name->setText(QCoreApplication::translate("MainWindow", "   \345\220\215\347\247\260", nullptr));
        label_status->setText(QCoreApplication::translate("MainWindow", "\347\212\266\346\200\201", nullptr));
        label_process->setText(QCoreApplication::translate("MainWindow", " \350\277\233\345\272\246", nullptr));
        label_size->setText(QCoreApplication::translate("MainWindow", "\345\244\247\345\260\217", nullptr));
        label_localPath->setText(QCoreApplication::translate("MainWindow", "\346\234\254\345\234\260\350\267\257\345\276\204", nullptr));
        label_remotePath->setText(QCoreApplication::translate("MainWindow", "\350\277\234\347\250\213\350\267\257\345\276\204", nullptr));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_transporting), QCoreApplication::translate("MainWindow", "\344\274\240\350\276\223\344\270\255", nullptr));
        label_fname->setText(QCoreApplication::translate("MainWindow", "   \345\220\215\347\247\260", nullptr));
        label_fstatus->setText(QCoreApplication::translate("MainWindow", "\347\212\266\346\200\201", nullptr));
        label_flocalPath->setText(QCoreApplication::translate("MainWindow", "\346\234\254\345\234\260\350\267\257\345\276\204", nullptr));
        label_fremotePath->setText(QCoreApplication::translate("MainWindow", "\350\277\234\347\250\213\350\267\257\345\276\204", nullptr));
        label_fsize->setText(QCoreApplication::translate("MainWindow", "\345\244\247\345\260\217", nullptr));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_finnished), QCoreApplication::translate("MainWindow", "\345\267\262\345\256\214\346\210\220", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
