/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLabel *label;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *qplot;
    QPushButton *lidar_start;
    QPushButton *lidar_mapping;
    QTextBrowser *txt_output;
    QPushButton *lidar_stop;
    QPushButton *lidar_mapping_clear;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *mappingplot;
    QPushButton *lidar_mapping_stop;
    QPushButton *lidar_mapping_save;
    QTextBrowser *car_sys_output;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *pclplot;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1055, 577);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(3);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Widget->sizePolicy().hasHeightForWidth());
        Widget->setSizePolicy(sizePolicy);
        Widget->setContextMenuPolicy(Qt::ActionsContextMenu);
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 0, 411, 42));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        QFont font;
        font.setPointSize(15);
        font.setBold(false);
        font.setItalic(true);
        font.setUnderline(false);
        font.setWeight(50);
        font.setStrikeOut(false);
        font.setStyleStrategy(QFont::PreferDefault);
        label->setFont(font);
        label->setCursor(QCursor(Qt::ArrowCursor));
        verticalLayoutWidget = new QWidget(Widget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(420, 370, 281, 191));
        qplot = new QVBoxLayout(verticalLayoutWidget);
        qplot->setObjectName(QString::fromUtf8("qplot"));
        qplot->setContentsMargins(0, 0, 0, 0);
        lidar_start = new QPushButton(Widget);
        lidar_start->setObjectName(QString::fromUtf8("lidar_start"));
        lidar_start->setGeometry(QRect(570, 50, 101, 31));
        lidar_mapping = new QPushButton(Widget);
        lidar_mapping->setObjectName(QString::fromUtf8("lidar_mapping"));
        lidar_mapping->setGeometry(QRect(690, 50, 101, 31));
        txt_output = new QTextBrowser(Widget);
        txt_output->setObjectName(QString::fromUtf8("txt_output"));
        txt_output->setGeometry(QRect(560, 240, 381, 51));
        lidar_stop = new QPushButton(Widget);
        lidar_stop->setObjectName(QString::fromUtf8("lidar_stop"));
        lidar_stop->setGeometry(QRect(570, 100, 101, 31));
        lidar_mapping_clear = new QPushButton(Widget);
        lidar_mapping_clear->setObjectName(QString::fromUtf8("lidar_mapping_clear"));
        lidar_mapping_clear->setGeometry(QRect(690, 150, 101, 31));
        verticalLayoutWidget_2 = new QWidget(Widget);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(10, 50, 381, 511));
        mappingplot = new QVBoxLayout(verticalLayoutWidget_2);
        mappingplot->setObjectName(QString::fromUtf8("mappingplot"));
        mappingplot->setContentsMargins(0, 0, 0, 0);
        lidar_mapping_stop = new QPushButton(Widget);
        lidar_mapping_stop->setObjectName(QString::fromUtf8("lidar_mapping_stop"));
        lidar_mapping_stop->setGeometry(QRect(690, 100, 101, 31));
        lidar_mapping_save = new QPushButton(Widget);
        lidar_mapping_save->setObjectName(QString::fromUtf8("lidar_mapping_save"));
        lidar_mapping_save->setGeometry(QRect(690, 200, 101, 31));
        car_sys_output = new QTextBrowser(Widget);
        car_sys_output->setObjectName(QString::fromUtf8("car_sys_output"));
        car_sys_output->setGeometry(QRect(560, 300, 381, 51));
        verticalLayoutWidget_3 = new QWidget(Widget);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(730, 370, 281, 191));
        pclplot = new QVBoxLayout(verticalLayoutWidget_3);
        pclplot->setObjectName(QString::fromUtf8("pclplot"));
        pclplot->setContentsMargins(0, 0, 0, 0);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label->setText(QCoreApplication::translate("Widget", "Welcome to real-time embedded system Cart !", nullptr));
        lidar_start->setText(QCoreApplication::translate("Widget", "Lidar start", nullptr));
        lidar_mapping->setText(QCoreApplication::translate("Widget", "mapping", nullptr));
        txt_output->setHtml(QCoreApplication::translate("Widget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Lidar status: free</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">mode: free</p></body></html>", nullptr));
        lidar_stop->setText(QCoreApplication::translate("Widget", "Lidar stop", nullptr));
        lidar_mapping_clear->setText(QCoreApplication::translate("Widget", "map clean", nullptr));
        lidar_mapping_stop->setText(QCoreApplication::translate("Widget", "map stop", nullptr));
        lidar_mapping_save->setText(QCoreApplication::translate("Widget", "map save", nullptr));
        car_sys_output->setHtml(QCoreApplication::translate("Widget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">current x: 0</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">current y: 0 </p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
