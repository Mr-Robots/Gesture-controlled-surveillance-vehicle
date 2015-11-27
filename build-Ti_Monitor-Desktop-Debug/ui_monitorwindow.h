/********************************************************************************
** Form generated from reading UI file 'monitorwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MONITORWINDOW_H
#define UI_MONITORWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Monitorwindow
{
public:
    QWidget *centralWidget;
    QProgressBar *Status_sensor2;
    QProgressBar *Status_sensor1;
    QTextBrowser *Sensor3;
    QLabel *label_Sensor3;
    QLabel *label_Sensor4;
    QTextBrowser *Sensor4_1;
    QProgressBar *Status_sensor3;
    QTextBrowser *Sensor2;
    QTextBrowser *Sensor1;
    QLabel *label_Sensor1;
    QLabel *label_Sensor2;
    QLabel *IR1;
    QLabel *IR2;
    QLabel *IR3;
    QTextBrowser *Sensor4_2;
    QTextBrowser *Sensor4_3;
    QLabel *label_Sensor4_2;
    QLabel *label_Sensor4_3;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Monitorwindow)
    {
        if (Monitorwindow->objectName().isEmpty())
            Monitorwindow->setObjectName(QStringLiteral("Monitorwindow"));
        Monitorwindow->resize(680, 489);
        Monitorwindow->setMinimumSize(QSize(680, 489));
        Monitorwindow->setMaximumSize(QSize(680, 489));
        Monitorwindow->setWindowOpacity(0.9);
        Monitorwindow->setStyleSheet(QLatin1String("QMainWindow\n"
"{\n"
"    background-image: url(:/new/Background/bg4.png);\n"
"}"));
        centralWidget = new QWidget(Monitorwindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        Status_sensor2 = new QProgressBar(centralWidget);
        Status_sensor2->setObjectName(QStringLiteral("Status_sensor2"));
        Status_sensor2->setGeometry(QRect(360, 130, 291, 31));
        Status_sensor2->setValue(24);
        Status_sensor2->setTextVisible(false);
        Status_sensor2->setOrientation(Qt::Horizontal);
        Status_sensor1 = new QProgressBar(centralWidget);
        Status_sensor1->setObjectName(QStringLiteral("Status_sensor1"));
        Status_sensor1->setGeometry(QRect(360, 50, 291, 31));
        Status_sensor1->setMinimum(2);
        Status_sensor1->setMaximum(150);
        Status_sensor1->setValue(1);
        Status_sensor1->setTextVisible(false);
        Status_sensor1->setOrientation(Qt::Horizontal);
        Sensor3 = new QTextBrowser(centralWidget);
        Sensor3->setObjectName(QStringLiteral("Sensor3"));
        Sensor3->setGeometry(QRect(290, 210, 61, 31));
        QFont font;
        font.setPointSize(13);
        Sensor3->setFont(font);
        label_Sensor3 = new QLabel(centralWidget);
        label_Sensor3->setObjectName(QStringLiteral("label_Sensor3"));
        label_Sensor3->setGeometry(QRect(20, 210, 221, 32));
        QFont font1;
        font1.setPointSize(15);
        label_Sensor3->setFont(font1);
        label_Sensor4 = new QLabel(centralWidget);
        label_Sensor4->setObjectName(QStringLiteral("label_Sensor4"));
        label_Sensor4->setGeometry(QRect(20, 290, 51, 31));
        label_Sensor4->setFont(font1);
        Sensor4_1 = new QTextBrowser(centralWidget);
        Sensor4_1->setObjectName(QStringLiteral("Sensor4_1"));
        Sensor4_1->setGeometry(QRect(230, 290, 121, 31));
        Sensor4_1->setFont(font);
        Status_sensor3 = new QProgressBar(centralWidget);
        Status_sensor3->setObjectName(QStringLiteral("Status_sensor3"));
        Status_sensor3->setGeometry(QRect(360, 210, 291, 31));
        Status_sensor3->setMinimum(0);
        Status_sensor3->setMaximum(100);
        Status_sensor3->setValue(-1);
        Status_sensor3->setTextVisible(false);
        Status_sensor3->setOrientation(Qt::Horizontal);
        Sensor2 = new QTextBrowser(centralWidget);
        Sensor2->setObjectName(QStringLiteral("Sensor2"));
        Sensor2->setGeometry(QRect(290, 130, 61, 31));
        Sensor2->setFont(font);
        Sensor1 = new QTextBrowser(centralWidget);
        Sensor1->setObjectName(QStringLiteral("Sensor1"));
        Sensor1->setGeometry(QRect(290, 50, 61, 31));
        Sensor1->setFont(font);
        label_Sensor1 = new QLabel(centralWidget);
        label_Sensor1->setObjectName(QStringLiteral("label_Sensor1"));
        label_Sensor1->setGeometry(QRect(20, 50, 271, 32));
        label_Sensor1->setFont(font1);
        label_Sensor2 = new QLabel(centralWidget);
        label_Sensor2->setObjectName(QStringLiteral("label_Sensor2"));
        label_Sensor2->setGeometry(QRect(20, 130, 151, 32));
        label_Sensor2->setFont(font1);
        IR1 = new QLabel(centralWidget);
        IR1->setObjectName(QStringLiteral("IR1"));
        IR1->setGeometry(QRect(90, 380, 71, 61));
        IR1->setStyleSheet(QLatin1String("#bulbglow\n"
"{\n"
"	border-image : url(:/basic/images/bulb_off.png);\n"
"}"));
        IR2 = new QLabel(centralWidget);
        IR2->setObjectName(QStringLiteral("IR2"));
        IR2->setGeometry(QRect(300, 380, 71, 61));
        IR2->setStyleSheet(QLatin1String("#bulbglow\n"
"{\n"
"	border-image : url(:/basic/images/bulb_off.png);\n"
"}"));
        IR3 = new QLabel(centralWidget);
        IR3->setObjectName(QStringLiteral("IR3"));
        IR3->setGeometry(QRect(510, 380, 71, 61));
        IR3->setStyleSheet(QLatin1String("#bulbglow\n"
"{\n"
"	border-image : url(:/basic/images/bulb_off.png);\n"
"}"));
        Sensor4_2 = new QTextBrowser(centralWidget);
        Sensor4_2->setObjectName(QStringLiteral("Sensor4_2"));
        Sensor4_2->setGeometry(QRect(230, 330, 121, 31));
        Sensor4_2->setFont(font);
        Sensor4_3 = new QTextBrowser(centralWidget);
        Sensor4_3->setObjectName(QStringLiteral("Sensor4_3"));
        Sensor4_3->setGeometry(QRect(360, 290, 291, 71));
        QFont font2;
        font2.setPointSize(10);
        Sensor4_3->setFont(font2);
        label_Sensor4_2 = new QLabel(centralWidget);
        label_Sensor4_2->setObjectName(QStringLiteral("label_Sensor4_2"));
        label_Sensor4_2->setGeometry(QRect(100, 290, 121, 21));
        label_Sensor4_2->setFont(font1);
        label_Sensor4_3 = new QLabel(centralWidget);
        label_Sensor4_3->setObjectName(QStringLiteral("label_Sensor4_3"));
        label_Sensor4_3->setGeometry(QRect(100, 330, 121, 21));
        label_Sensor4_3->setFont(font1);
        Monitorwindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Monitorwindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Monitorwindow->setStatusBar(statusBar);

        retranslateUi(Monitorwindow);

        QMetaObject::connectSlotsByName(Monitorwindow);
    } // setupUi

    void retranslateUi(QMainWindow *Monitorwindow)
    {
        Monitorwindow->setWindowTitle(QApplication::translate("Monitorwindow", "Monitorwindow", 0));
        label_Sensor3->setText(QApplication::translate("Monitorwindow", "Light Sensor :", 0));
        label_Sensor4->setText(QApplication::translate("Monitorwindow", "GPS", 0));
        label_Sensor1->setText(QApplication::translate("Monitorwindow", "Temperature Sensor (*C) : ", 0));
        label_Sensor2->setText(QApplication::translate("Monitorwindow", "Gas Sensor :", 0));
        IR1->setText(QString());
        IR2->setText(QString());
        IR3->setText(QString());
        label_Sensor4_2->setText(QApplication::translate("Monitorwindow", "Latitude", 0));
        label_Sensor4_3->setText(QApplication::translate("Monitorwindow", "Longitude", 0));
    } // retranslateUi

};

namespace Ui {
    class Monitorwindow: public Ui_Monitorwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MONITORWINDOW_H
