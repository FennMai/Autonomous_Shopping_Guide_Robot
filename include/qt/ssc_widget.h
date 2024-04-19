#ifndef SSC_WIDGET_H
#define SSC_WIDGET_H

#include <QWidget>
#include <QVector>
#include "qcustomplot.h"
#include <thread>
#include <chrono>
#include <QDebug>
#include <unistd.h>

#include <cmath>
#include <QTimer>
#include <QMutex>
#include "sl_lidar.h"
#include "sl_lidar_driver.h"
#include "PCLDetect.h"
#include "CarControl.hpp"

#include <QLabel>
#include <QMovie>

QT_BEGIN_NAMESPACE
namespace Ui { class SSC_Widget; }
QT_END_NAMESPACE

struct RT_location {
    float x;
    float y;
    float orientation;
};

class SSC_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit SSC_Widget(QWidget *parent = nullptr);
    ~SSC_Widget();

    void setupPlot(); // 声明一个函数来设置和初始化 QCustomPlot
    // lidar drive
    bool initializeLidar(const char* port, sl_u32 baudrate);
    bool checkSLAMTECLIDARHealth();
    void startLidarScan();
    void stopLidarScan();
    void lidarmapping();
    void getData();
    void saveLidarData(const QString &filePath);

public slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void handleAnimationFinished();
    void checkLastFrame(int frameNumber);

private:
    Ui::SSC_Widget *ui;
    QLabel *animationLabel;
    QMovie *animationMovie;

    CarControl* carControl;
    QCustomPlot *LidarPlot; // 添加一个 QCustomPlot 指针作为私有成员
    QCustomPlot *MapPlot;
    QCustomPlot *pclplot;
    QVector<double> cloud_x;
    QVector<double> cloud_y;
    int scatter_size = 30;
    int currentIndex = 0; // 作为调用 timerEvent()的参数之一，直接结束定时器的参数

    // 设置一个多线程
    std::thread plotThread;
    bool keepPlotting = true;

    sl::ILidarDriver *drv;
    bool ctrl_c_pressed = false;
    // basic drive static parameter
    static const unsigned nDistance = 8192;
    sl_u32 baudrate = 460800;
    const char *netport = "/dev/ttyUSB0";

    QTimer *dataTimer; // 这个学习一下
    QVector<LidarPoint> pathMappingPoints;
    QMutex dataMutex;

    int lidar_cal_range_low = 0;
    int lidar_cal_range_upper = 4000;
    RT_location rt_location;

    PCLDetect pclDetector;
};
#endif // SSC_WIDGET_H
