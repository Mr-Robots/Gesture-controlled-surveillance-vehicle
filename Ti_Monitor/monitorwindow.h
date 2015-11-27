#ifndef MONITORWINDOW_H
#define MONITORWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtCore>
#include <QThread>
#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include <unistd.h>
#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <QtMultimedia/QMediaPlayer>
#include <iostream>

#define DebuG true

namespace Ui
{
    class Monitorwindow;
}

class workthread : public QThread
{
    Q_OBJECT

    public:
        workthread() {};
        ~workthread() {};
        void run();

    public slots:
        void readData();
};

class Monitorwindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Monitorwindow(QWidget *parent = 0);
        ~Monitorwindow();
        workthread port_thread;
        workthread location_thread;
        workthread socket_port_thread;
        workthread gesture_port_thread;
        workthread gesture_server_thread;

    public slots:
        void write_on_statusbar();
        void display_sensor_data();

    protected:
        void changeEvent(QEvent *e);

    private:
        void portscan(void);
        Ui::Monitorwindow *ui;
};

#endif // MONITORWINDOW_H
