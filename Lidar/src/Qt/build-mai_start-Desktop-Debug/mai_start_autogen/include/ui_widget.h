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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Cart
{
public:
    QLabel *label;
    QPushButton *Button_lidar;
    QPushButton *Button_closed;
    QLabel *label_2;

    void setupUi(QWidget *Cart)
    {
        if (Cart->objectName().isEmpty())
            Cart->setObjectName(QString::fromUtf8("Cart"));
        Cart->resize(802, 600);
        QFont font;
        font.setBold(false);
        font.setItalic(true);
        font.setUnderline(false);
        font.setWeight(50);
        font.setStrikeOut(false);
        font.setStyleStrategy(QFont::PreferDefault);
        Cart->setFont(font);
        Cart->setCursor(QCursor(Qt::ArrowCursor));
        Cart->setMouseTracking(false);
        Cart->setAutoFillBackground(false);
        label = new QLabel(Cart);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 411, 42));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setPointSize(15);
        font1.setBold(false);
        font1.setItalic(true);
        font1.setUnderline(false);
        font1.setWeight(50);
        font1.setStrikeOut(false);
        font1.setStyleStrategy(QFont::PreferDefault);
        label->setFont(font1);
        label->setCursor(QCursor(Qt::ArrowCursor));
        Button_lidar = new QPushButton(Cart);
        Button_lidar->setObjectName(QString::fromUtf8("Button_lidar"));
        Button_lidar->setGeometry(QRect(610, 84, 101, 31));
        QFont font2;
        font2.setPointSize(11);
        font2.setBold(false);
        font2.setItalic(false);
        font2.setUnderline(false);
        font2.setWeight(50);
        font2.setStrikeOut(false);
        font2.setStyleStrategy(QFont::PreferDefault);
        Button_lidar->setFont(font2);
        Button_lidar->setCursor(QCursor(Qt::ArrowCursor));
        Button_lidar->setFocusPolicy(Qt::WheelFocus);
        Button_lidar->setAutoRepeatDelay(296);
        Button_closed = new QPushButton(Cart);
        Button_closed->setObjectName(QString::fromUtf8("Button_closed"));
        Button_closed->setGeometry(QRect(620, 370, 121, 25));
        Button_closed->setFont(font2);
        Button_closed->setCursor(QCursor(Qt::ArrowCursor));
        Button_closed->setFocusPolicy(Qt::WheelFocus);
        Button_closed->setAutoRepeatDelay(296);
        label_2 = new QLabel(Cart);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 90, 381, 41));
        label_2->setFont(font1);

        retranslateUi(Cart);

        QMetaObject::connectSlotsByName(Cart);
    } // setupUi

    void retranslateUi(QWidget *Cart)
    {
        Cart->setWindowTitle(QCoreApplication::translate("Cart", "Widget", nullptr));
        label->setText(QCoreApplication::translate("Cart", "Welcome to real-time embedded system Cart !", nullptr));
        Button_lidar->setText(QCoreApplication::translate("Cart", "Lidar connect", nullptr));
        Button_closed->setText(QCoreApplication::translate("Cart", "Close Window", nullptr));
        label_2->setText(QCoreApplication::translate("Cart", "Welcome to real-time embedded system!", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Cart: public Ui_Cart {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
