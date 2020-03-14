#ifndef FTPMAIN_H
#define FTPMAIN_H

#include <QMainWindow>

namespace Ui {
class FtpMain;
}

class FtpMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit FtpMain(QWidget *parent = nullptr);
    ~FtpMain();

private:
    Ui::FtpMain *ui;
};

#endif // FTPMAIN_H
