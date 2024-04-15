/********************************************************************************
** Form generated from reading UI file 'ssc_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SSC_WIDGET_H
#define UI_SSC_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SSC_Widget
{
public:
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *SSC_Widget)
    {
        if (SSC_Widget->objectName().isEmpty())
            SSC_Widget->setObjectName(QString::fromUtf8("SSC_Widget"));
        SSC_Widget->resize(800, 480);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(2);
        sizePolicy.setHeightForWidth(SSC_Widget->sizePolicy().hasHeightForWidth());
        SSC_Widget->setSizePolicy(sizePolicy);
        label = new QLabel(SSC_Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 301, 41));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        QFont font;
        font.setPointSize(13);
        font.setBold(true);
        font.setItalic(true);
        font.setWeight(75);
        label->setFont(font);
        label_2 = new QLabel(SSC_Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(670, 440, 121, 41));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        label_2->setFont(font1);

        retranslateUi(SSC_Widget);

        QMetaObject::connectSlotsByName(SSC_Widget);
    } // setupUi

    void retranslateUi(QWidget *SSC_Widget)
    {
        SSC_Widget->setWindowTitle(QCoreApplication::translate("SSC_Widget", "SSC_Widget", nullptr));
        label->setText(QCoreApplication::translate("SSC_Widget", "Welcome to the smart shopping cart!", nullptr));
        label_2->setText(QCoreApplication::translate("SSC_Widget", "author\357\274\232FennMai", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SSC_Widget: public Ui_SSC_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SSC_WIDGET_H
