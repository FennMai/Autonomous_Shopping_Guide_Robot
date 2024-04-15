#ifndef WIDGET_H
#define WIDGET_H
//#define QT_NO_DEBUG_OUTPUT

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
#include "sound.h"



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

struct LidarPoint {
    float phi;
    float r;
    float x;
    float y;
    unsigned int signal_strength;
    bool valid;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


    void setupPlot(); // 声明一个函数来设置和初始化 QCustomPlot
    // lidar drive
    bool initializeLidar(const char* port, sl_u32 baudrate);
    bool checkSLAMTECLIDARHealth();
    void getData();
    void startLidarScan();
    void stopLidarScan();


private slots:

    void on_lidar_mapping_clicked();

    void on_lidar_start_clicked();

    void on_lidar_stop_clicked();

    void on_lidar_mapping_clear_clicked();

private:
    Ui::Widget *ui;
    QCustomPlot *LidarPlot; // 添加一个 QCustomPlot 指针作为私有成员

    QVector<double> cloud_x;
    QVector<double> cloud_y;
    int scatter_size =30;
    int currentIndex = 0; // 作为调用 timerEvent()的参数之一，直接结束定时器的参数

    // 设置一个多线程
    std::thread plotThread;
    bool keepPlotting = true;
    void plotData(); // Method to handle plotting in a separate thread

    sl::ILidarDriver *drv;
    bool ctrl_c_pressed = false;
    // basic drive static parameter
    static const unsigned nDistance = 8192;
    sl_u32 baudrate = 460800;
    const char *netport = "/dev/ttyUSB0";

    QTimer *dataTimer; // 这个学习一下
    QVector<LidarPoint> lidarData;
    QMutex dataMutex;
    // sound

    SoundManager *soundManager;  // SoundManager实例，用于管理音频设备操作
    QTimer *timer;  // 用于定时调用更新函数
    void updateCMDIDDisplay(); // 是否要要slot
};


#endif // WIDGET_H
