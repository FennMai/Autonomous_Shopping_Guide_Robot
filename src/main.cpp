//df
#include "ssc_widget.h"
#include"CarControl.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SSC_Widget w;
    w.show();
    return a.exec();
}
