/**
 * author : ZMai
 * last modified date: 14/04/2024
 * last commit:
 * driving files:
 *
 * basic 1.1, date:10/04/2024:
 * basic from the simple_ultra.cpp, the change is the env --- from makefile to camkelist.txt
 *
 * basic 1.2, date:10/04/2024:
 * Refining code
 *
 * basic 2.0, date:12/04/2024:
 * Refining code，successing drive the lidar via qt
 *
 * basic 2.1, date:13/04/2024:
 * basic lidar data filter and plot in qt in real time
 * vector name：customPlot change into LidarPlot
 *
 *
 **/

#include "ssc_widget.h"
#include "./ui_ssc_widget.h"
#include "CarControl.hpp"
#include <QLabel>
#include <QMovie>


SSC_Widget::SSC_Widget(QWidget *parent) : QWidget(parent), ui(new Ui::SSC_Widget) {
    ui->setupUi(this);
    // 设置窗口为无边框和全屏
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    showFullScreen();
    // 初始化 GIF 动画

//    animationLabel = new QLabel(this);
    animationLabel = ui->label_3;
    animationLabel->setScaledContents(true);
    animationMovie = new QMovie("/home/team2/04199/start.gif");
    animationLabel->setMovie(animationMovie);
    animationMovie->start();
    //animationLabel->setGeometry(0, 0, 800, 480);  // Adjust as needed

    // 连接信号与槽来检查帧的变化
    connect(animationMovie, &QMovie::frameChanged, this, &SSC_Widget::checkLastFrame);
    connect(animationMovie, &QMovie::finished, this, &SSC_Widget::handleAnimationFinished);

    // 初始化 QCustomPlot 对象
    LidarPlot = new QCustomPlot();
    ui->LidarPlot->addWidget(LidarPlot);
    MapPlot = new QCustomPlot();
    ui->MapPlot->addWidget(MapPlot);
    pclplot = new QCustomPlot();
    ui->pclplot->addWidget(pclplot);
    setupPlot();

    // 创建激光雷达驱动实例
    drv = *sl::createLidarDriver();
    if (!drv) {
        qDebug() << "Insufficient memory, exit";
        exit(-1);
    }
    if (!initializeLidar("/dev/ttyUSB0", 115200) || !checkSLAMTECLIDARHealth()) {
        delete drv;
        drv = nullptr;
        qDebug() << "Lidar initialization failed";
        exit(-1);
    }

    // Setup data timer for live lidar data processing
    dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, this, &SSC_Widget::getData);

    // Initialize car control
    carControl = CarControl::getInstance();
    carControl->initialize();
}

SSC_Widget::~SSC_Widget() {
    qDebug() << "Destroying Widget, stopping timer.";
    if (drv) {
        drv->stop();
        delete drv;
    }
    drv = nullptr;
    delete ui;
    delete LidarPlot;
    delete MapPlot;
    delete pclplot;
    delete dataTimer;
    delete animationLabel;
    delete animationMovie;
}

void SSC_Widget::setupPlot() {
    // Configure the plots, such as LidarPlot, MapPlot, and pclplot
    // Detailed setup code is assumed here based on your previous settings
}

bool SSC_Widget::initializeLidar(const char* port, sl_u32 baudrate) {
    // Initialize the lidar device with the provided port and baudrate
    return true;  // Simplified return statement
}

bool SSC_Widget::checkSLAMTECLIDARHealth() {
    // Check the health of the lidar device
    return true;  // Simplified return statement
}

void SSC_Widget::startLidarScan() {
    // Start the lidar scanning process
}

void SSC_Widget::stopLidarScan() {
    // Stop the lidar scanning process
}

void SSC_Widget::lidarmapping() {
    // Mapping function for lidar data
}

void SSC_Widget::getData() {
    // Function to retrieve data from lidar
}

void SSC_Widget::saveLidarData(const QString &filePath) {
    // Save lidar data to specified file path
}

void SSC_Widget::on_pushButton_clicked() {
    // Handler for first button click
}

void SSC_Widget::on_pushButton_2_clicked() {
    // Handler for second button click
}
void SSC_Widget::on_pushButton_3_clicked() {
    qDebug() << "Button 3 clicked";
    // 添加按钮3点击的逻辑处理
}

void SSC_Widget::on_pushButton_4_clicked() {
    qDebug() << "Button 4 clicked";
    // 添加按钮4点击的逻辑处理
}
void SSC_Widget::handleAnimationFinished() {
    // Hide the animation label after the GIF has finished playing
    animationLabel->hide();
    qDebug() << "Animation finished and label hidden.";

}


void SSC_Widget::checkLastFrame(int frameNumber) {
    if (frameNumber == animationMovie->frameCount() - 1) {
        animationMovie->stop();
        animationLabel->hide();
    }
}
