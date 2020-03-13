#ifndef CONNECT_SERVER_H
#define CONNECT_SERVER_H

#include <QDialog>

namespace Ui {
class connect_server;
}

class connect_server : public QDialog
{
    Q_OBJECT

public:
    explicit connect_server(QWidget *parent = nullptr);
    ~connect_server();

private:
    Ui::connect_server *ui;
};

#endif // CONNECT_SERVER_H
