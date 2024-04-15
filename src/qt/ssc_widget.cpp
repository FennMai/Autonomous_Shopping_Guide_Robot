#include "ssc_widget.h"
#include "./ui_ssc_widget.h"

SSC_Widget::SSC_Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SSC_Widget)
{
    ui->setupUi(this);
}

SSC_Widget::~SSC_Widget()
{
    delete ui;
}

