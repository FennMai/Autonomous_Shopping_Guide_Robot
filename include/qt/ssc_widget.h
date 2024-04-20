#ifndef SSC_WIDGET_H
#define SSC_WIDGET_H

#include <QWidget>
#include <QVector>
#include <thread>
#include <chrono>
#include <QDebug>
#include <unistd.h>
#include <cmath>
#include <QTimer>
#include <QMutex>
#include "qcustomplot.h"
#include "sl_lidar.h"
#include "sl_lidar_driver.h"
#include "PCLDetect.h"
#include "CarControl.hpp"
#include "sound.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SSC_Widget; }
QT_END_NAMESPACE

struct RT_location {
    float x = 0;
    float y = 0;
    float orientation;
};
class SSC_Widget : public QWidget
{
    Q_OBJECT

public:
    SSC_Widget(QWidget *parent = nullptr);
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
    //
    void car_drive(bool ob_detect_050);

    void vocie_update();
    void processCommand(int cmdid);

public slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void checkLastFrame(int frameNumber);

private:
    Ui::SSC_Widget *ui;
    // 开场动画
    QLabel *animationLabel;
    QMovie *animationMovie;

    CarControl* carControl;
    QCustomPlot *LidarPlot; // 添加一个 QCustomPlot 指针作为私有成员
    QCustomPlot *MapPlot;
    QCustomPlot *pclplot;
    QVector<double> cloud_x;
    QVector<double> cloud_y;
//    int scatter_size =30;
    int currentIndex = 0; // 作为调用 timerEvent()的参数之一，直接结束定时器的参数

    // 设置一个多线程
//    std::thread plotThread;
//    bool keepPlotting = true;
//    void plotData(); // Method to handle plotting in a separate thread

    sl::ILidarDriver *drv;
//    std::unique_ptr<sl::ILidarDriver> drv;
    bool ob_detect_050= false;
    // basic drive static parameter
    static const unsigned nDistance = 8192;
    sl_u32 baudrate = 460800;
    const char *netport = "/dev/ttyUSB0";
    QVector<LidarPoint> pathMappingPoints;

    QTimer *dataTimer;
    QTimer *modeTimer;
    QMutex dataMutex;

    int lidar_cal_range_low = 0;
    int lidar_cal_range_upper = 3*4000;
    RT_location rt_location;

    PCLDetect pclDetector; // dont use it now
    // sound sensor
    SoundManager *soundManager;
    int cmdid;

};
#endif // SSC_WIDGET_H
