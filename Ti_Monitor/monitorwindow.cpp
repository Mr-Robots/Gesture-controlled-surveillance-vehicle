////////////////    Header Files    ////////////////
#include "monitorwindow.h"
#include "ui_monitorwindow.h"

////////////////   Variables Init   ////////////////
QSerialPort *serial, *serial_gesture;
QString status_bar_data;
QByteArray port_data, gesture_port_data;
QAction *status_changed, *sensor_data_changed;
unsigned char sensor_data_bytes[20], gesture_data_bytes[1050];
int thread_status;
unsigned long long sensor_data[10];
float GPS_lat, GPS_lon, pre_GPS_lat, pre_GPS_lon;
char location_data[500];
bool GPS_changed = true;
enum{port_read, get_location, socket_port, gesture_port, gesture_server};

Monitorwindow::Monitorwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Monitorwindow)
{
    ui->setupUi(this);

    status_changed = new QAction(this);
    sensor_data_changed = new QAction(this);
    serial = new QSerialPort(this);
    serial_gesture = new QSerialPort(this);

    connect(status_changed, SIGNAL(triggered()), this, SLOT(write_on_statusbar()));
    connect(sensor_data_changed, SIGNAL(triggered()), this, SLOT(display_sensor_data()));

    ui->Status_sensor1->setStyleSheet(tr("QProgressBar{ border: 2px solid grey; border-radius: 5px; }  QProgressBar::chunk { background-color: #0000FF; width: 10px; margin: 2px; }"));
    ui->Status_sensor2->setStyleSheet(tr("QProgressBar{ border: 2px solid grey; border-radius: 5px; }  QProgressBar::chunk { background-color: #0000FF; width: 10px; margin: 2px; }"));
    ui->Status_sensor3->setStyleSheet(tr("QProgressBar{ border: 2px solid grey; border-radius: 5px; }  QProgressBar::chunk { background-color: #0000FF; width: 10px; margin: 2px; }"));
    ui->IR1->setStyleSheet(tr("#IR1\n{\n	border-image : url(:/new/Background/bulb_on.png);\n}"));
    ui->IR2->setStyleSheet(tr("#IR2\n{\n	border-image : url(:/new/Background/bulb_on.png);\n}"));
    ui->IR3->setStyleSheet(tr("#IR3\n{\n	border-image : url(:/new/Background/bulb_on.png);\n}"));

    thread_status = gesture_port;
    gesture_port_thread.start();
    QThread::msleep(10);
    thread_status = gesture_server;
    gesture_server_thread.start();
    QThread::msleep(10);
    thread_status = socket_port;
    socket_port_thread.start();
    QThread::msleep(10);
    thread_status = port_read;
    port_thread.start();
//    QThread::msleep(10);
//    thread_status = get_location;
//    location_thread.start();
}

void Monitorwindow::display_sensor_data()
{
    char temp[100];

    ////////////////////////  IR Sensors  ////////////////////////
    if(sensor_data_bytes[12] & ((unsigned char)1<<2))
        ui->IR1->setStyleSheet(tr("#IR1\n{\n	border-image : url(:/new/Background/bulb_on.png);\n}"));
    else
        ui->IR1->setStyleSheet(tr("#IR1\n{\n	border-image : url(:/new/Background/bulb_red.png);\n}"));
    if(sensor_data_bytes[12] & ((unsigned char)1<<3))
        ui->IR2->setStyleSheet(tr("#IR2\n{\n	border-image : url(:/new/Background/bulb_on.png);\n}"));
    else
        ui->IR2->setStyleSheet(tr("#IR2\n{\n	border-image : url(:/new/Background/bulb_red.png);\n}"));
    if(sensor_data_bytes[12] & ((unsigned char)1<<4))
        ui->IR3->setStyleSheet(tr("#IR3\n{\n	border-image : url(:/new/Background/bulb_on.png);\n}"));
    else
        ui->IR3->setStyleSheet(tr("#IR3\n{\n	border-image : url(:/new/Background/bulb_red.png);\n}"));

    ////////////////////////  Temperature Sensor  ////////////////////////
    sensor_data[0] = sensor_data[0]/12;
    sprintf(temp,"%llu",sensor_data[0]);
    ui->Sensor1->setText(temp);
    ui->Status_sensor1->setValue(sensor_data[0]);
    if(sensor_data[0] > 60)
        ui->Status_sensor1->setStyleSheet(tr("QProgressBar{ border: 2px solid grey; border-radius: 5px; }  QProgressBar::chunk { background-color: #FF0000; width: 10px; margin: 2px; }"));
    else
        ui->Status_sensor1->setStyleSheet(tr("QProgressBar{ border: 2px solid grey; border-radius: 5px; }  QProgressBar::chunk { background-color: #0000FF; width: 10px; margin: 2px; }"));

    ////////////////////////  Gas Sensor  ////////////////////////
    sensor_data[1]-=2795;
    sensor_data[1]/=13;
    sprintf(temp,"%llu",sensor_data[1]);
    ui->Sensor2->setText(temp);
    ui->Status_sensor2->setValue(sensor_data[1]);
    if(sensor_data[1] > 70)
        ui->Status_sensor2->setStyleSheet(tr("QProgressBar{ border: 2px solid grey; border-radius: 5px; }  QProgressBar::chunk { background-color: #FF0000; width: 10px; margin: 2px; }"));
    else
        ui->Status_sensor2->setStyleSheet(tr("QProgressBar{ border: 2px solid grey; border-radius: 5px; }  QProgressBar::chunk { background-color: #0000FF; width: 10px; margin: 2px; }"));

    ////////////////////////  Light Sensor  ////////////////////////
    sensor_data[2]/=41;
    sprintf(temp,"%llu",sensor_data[2]);
    ui->Sensor3->setText(temp);
    ui->Status_sensor3->setValue(sensor_data[2]);
    if(sensor_data[2] > 80)
        ui->Status_sensor3->setStyleSheet(tr("QProgressBar{ border: 2px solid grey; border-radius: 5px; }  QProgressBar::chunk { background-color: #FF0000; width: 10px; margin: 2px; }"));
    else
        ui->Status_sensor3->setStyleSheet(tr("QProgressBar{ border: 2px solid grey; border-radius: 5px; }  QProgressBar::chunk { background-color: #0000FF; width: 10px; margin: 2px; }"));

    ////////////////////////  GPS Sensor  ////////////////////////
    sprintf(temp,"%f",GPS_lat);
    ui->Sensor4_1->setText(temp);
    sprintf(temp,"%f",GPS_lon);
    ui->Sensor4_2->setText(temp);
//    thread_status = get_location;
//    location_thread.start();
    if(GPS_lat || GPS_lon)
        ui->Sensor4_3->setText(tr(location_data));
    else
        ui->Sensor4_3->setText(tr("Message : Waiting for GPS response!"));
}

void Monitorwindow::write_on_statusbar()
{
    ui->statusBar->showMessage(status_bar_data,5000);
}

void workthread::run()
{
    switch(thread_status)
    {
        case socket_port:
            system("socat -d -d pty,link=/dev/ttyUSB0,raw,echo=0,waitslave tcp:192.168.150.10:7000");
            break;

        case port_read:
            while(1)
            {
                if(!QSerialPortInfo::availablePorts().isEmpty())
                {
                    serial->setPortName("ttyUSB0");
//                    serial->setPortName(QSerialPortInfo::availablePorts().last().portName());
                    serial->setBaudRate(QSerialPort::Baud9600);
                    serial->setDataBits(QSerialPort::Data8);
                    serial->setParity(QSerialPort::NoParity);
                    serial->setStopBits(QSerialPort::OneStop);
                    serial->setFlowControl(QSerialPort::NoFlowControl);
                    if (serial->open(QIODevice::ReadWrite))
                    {
                        while(1)
                        {
                            while(!serial->waitForReadyRead(5) && !QSerialPortInfo::availablePorts().isEmpty())
                            {
                                status_bar_data = tr("Error : No data on serialport!");
                                status_changed->trigger();
                                QThread::sleep(2);
                            }
                            status_bar_data = tr("");
                            status_changed->trigger();
                            if(QSerialPortInfo::availablePorts().isEmpty())
                                break;
                            port_data = serial->readAll();
                            if(port_data.contains(0xEF))
                            {
                                int i=0,j=0;
                                while(i<port_data.size() && port_data[i++] != (char)0xEF);
                                for(j=0; (i+j)<port_data.size() && j<13; ++j)
                                    sensor_data_bytes[j] = port_data[i+j];
                                if(j == 13 && port_data[i+j] == (char)0xFF)
                                {
                                    for(j=0; j<3; ++j)
                                        sensor_data[j] = sensor_data_bytes[2*j] + (sensor_data_bytes[2*j+1]*256);
                                    sensor_data[3] = sensor_data_bytes[6] + (sensor_data_bytes[7]*256) + (sensor_data_bytes[8]*65536);
                                    sensor_data[4] = sensor_data_bytes[9] + (sensor_data_bytes[10]*256) + (sensor_data_bytes[11]*65356);

                                    pre_GPS_lat = GPS_lat;
                                    pre_GPS_lon = GPS_lon;
                                    GPS_lat = (double)sensor_data[3]/100000;
                                    GPS_lon = (double)sensor_data[4]/100000;

                                    if(((int)pre_GPS_lat != (int)GPS_lat) || ((int)pre_GPS_lon != (int)GPS_lon))
                                        GPS_changed = true;
                                    else if(strcmp(location_data,"Error : can not find location!") == 0)
                                        GPS_changed = true;
                                    else
                                        GPS_changed = false;
                                    sensor_data_changed->trigger();
                                }
                                else
                                {
                                    qDebug() << "Lost 0xFF";
                                    serial->clear(QSerialPort::Input);
                                }
                            }
                            else
                            {
                                qDebug() << "Lost 0xEF";
                                serial->clear(QSerialPort::Input);
                            }
                            serial->clear(QSerialPort::Input);
                            QThread::msleep(600);
                        }
                    }
                    else
                    {
                        status_bar_data = tr("Error : Can not open serialport! ");
                        status_changed->trigger();
                        QThread::sleep(4);
                    }
                }
                while(QSerialPortInfo::availablePorts().isEmpty())
                {
                    status_bar_data = tr("Error : No serialport Found! ");
                    status_changed->trigger();
                    QThread::sleep(4);
                }
                status_bar_data = tr("Message : Serialport is active! ");
                status_changed->trigger();
            }
            break;


        case get_location:
            char buf[400];
            if(GPS_lat || GPS_lon)
            {
                sprintf(buf,"python ../Ti_Monitor/Geo_map.py %f,%f",GPS_lat,GPS_lon);
                FILE *f;
                f = popen(buf,"r");
                if(f)
                {
                    QThread::sleep(10);
                    location_data[0] = '\0';
                    int x = fread(location_data,sizeof(char),490,f);
                    location_data[x]=location_data[x+1]='\0';
                    pclose(f);
                }
                else
                {
                    strcpy(location_data,"");
                    status_bar_data = tr("Error : Can not Find location!");
                    status_changed->trigger();
                }
            }
            break;

        case gesture_port:
            system("python ../Ti_Monitor/Gesture_serial.py");
            break;

        case gesture_server:
            system("./gesture_server");
            break;
    };
}

void Monitorwindow::portscan(void)
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            serial.write("Port working!");
            serial.close();
        }
    }
}

Monitorwindow::~Monitorwindow()
{
    location_thread.terminate();
    socket_port_thread.terminate();
    port_thread.terminate();
    gesture_port_thread.terminate();
    gesture_server_thread.terminate();
    serial->close();
    serial_gesture->close();
    system("pkill socat");
//    system("pkill gesture_server");
    delete ui;
}

void workthread::readData()
{
    QByteArray data = serial->readAll();
    qDebug() << data.data() << endl;
}

void Monitorwindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}
