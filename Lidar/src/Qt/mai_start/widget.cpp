#include "widget.h"
#include "./ui_widget.h"
CppTimer.h
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
//    this->setGeometry(0, 0, 800, 480);

    pushButton1 = new QPushButton("COLOR1",this);
    pushButton1->setGeometry(300,200,80,40);

    connect(pushButton1, SIGNAL(clicked()), this, SLOT(pushbutton1_clicked()));

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_Button_closed_clicked()
{
    this->close();
}

void Widget::pushbutton1_clicked()
{
    this->setStyleSheet("QWidget { background-color: rgba(238, 122, 233, 100%); }");
}


void Widget::on_Button_lidar_clicked()
{
    ui->label_2->setText("starting connect ...");
}



