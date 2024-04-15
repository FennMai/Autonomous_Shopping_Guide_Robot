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
 * basic 2.2, date:14/04/2024:
 * designing the qt, add function and update plotting method
 *
 *
 **/


#include "widget.h"
#include "./ui_widget.h"
#include <QString>


Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget),soundManager(new SoundManager(1, 0x64))
{
    ui->setupUi(this);

    /***QCustomPlot 连接到qplot***/
    // 初始化 QCustomPlot 对象
    LidarPlot = new QCustomPlot(); // 不需要传递 this，因为后面会添加到布局中
    // 将 QCustomPlot 对象添加到已存在的 QVBoxLayout 中
    ui->qplot->addWidget(LidarPlot); // 使用 ui->qplot，不需要创建新的 QVBoxLayout
    /***调用 setupPlot 函数初始化图表设置***/
    setupPlot();

    // 直接使用多线程，缺点，它是计算好所有的，只是像动画一样展示，我需要的是，实时计算一个数据，实时展示
    //plotThread = std::thread([this] { this->plotData(); });

    /* lidar */
    // 创建激光雷达驱动实例
//    drv = *sl::createLidarDriver();
//    if (!drv) {
//        qDebug() << "Insufficient memory, exit";
//        exit(-1);
//    }
//    if (!initializeLidar(netport, baudrate) || !checkSLAMTECLIDARHealth()) {
//        delete drv;
//        drv = nullptr;
//        qDebug() << "Lidar initialization failed";
//        exit(-1);
//    }

    // 连接dataTimer的timeout信号到当前Widget的getData槽函数。
    // 当dataTimer发出timeout信号时（即每次计时器间隔结束时），将自动调用Widget的getData方法
    dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, this, &Widget::getData);


    // sound test
    try {
            soundManager->startCheckingCMDID();  // 开始检查CMDID
        } catch (const std::exception& e) {
            ui->txt_output->setText(QString::fromStdString(e.what()));
            return;
        }
    // setting timer
    timer = new QTimer(this);  // 创建一个新的QTimer对象
    connect(timer, &QTimer::timeout, this, &Widget::updateCMDIDDisplay);
    timer->start(1000);  // 设置定时器每1000毫秒（1秒）触发一次


}

Widget::~Widget()
{
    qDebug() << "Destroying Widget, stopping timer.";
    drv->stop();
    delete drv;
    drv = nullptr;
    delete soundManager;
    delete ui; // 删除 UI 对象，不需要单独删除 customPlot，因为它由 Qt 的父子关系自动管理
}

void Widget::updateCMDIDDisplay()
{
    // 从共享内存读取CMDID，并更新UI
    soundManager->updateOutputText([this](const std::string &text) {
        ui->txt_output->setText("当前状态是：" + QString::fromStdString(text));
    });
}


void Widget::setupPlot()
{
    // 配置 LidarPlot 显示范围,地图大小
    LidarPlot->xAxis->setRange(-1, 1);
    LidarPlot->yAxis->setRange(-1, 1);
    // 在这里配置 LidarPlot，设置轴标题
    LidarPlot->xAxis->setLabel("X Axis / cm ");
    LidarPlot->yAxis->setLabel("Y Axis / cm ");

    // 创建一个新的图形用于绘制数据
    LidarPlot->addGraph();
    LidarPlot->graph(0)->setName("Lidar Dynamic Data");

    // 绘图风格
    LidarPlot->graph(0)->setLineStyle(QCPGraph::lsNone); // 不使用线连接点
    QCPScatterStyle scatterStyle;
    // 设置形状为正方形
    // ssCircle 圆形
    scatterStyle.setShape(QCPScatterStyle::ssSquare);
    scatterStyle.setSize(5); // 设置点的大小
    scatterStyle.setPen(QPen(Qt::red)); // 设置点边界的颜色
    scatterStyle.setBrush(Qt::yellow); // 设置点的填充颜色
    LidarPlot->graph(0)->setScatterStyle(scatterStyle);
}

//void Widget::plotData(){
//    int index = 0;
//    while (keepPlotting) {
//        double x = index / 50.0 - 1;
//        double y = x * x;

//        // Use Qt's signal/slot mechanism to update the GUI from the worker thread
//        QMetaObject::invokeMethod(this, [this, x, y]() {
//            customPlot->graph(0)->addData(x, y);
//            customPlot->xAxis->setRange(x - 1, x + 1);
//            customPlot->replot();
//        }, Qt::QueuedConnection);

//        std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep for 1 second
//        index++;
//    }
//}

bool Widget::initializeLidar(const char* port, sl_u32 baudrate)
{
    sl::IChannel* channel = *sl::createSerialPortChannel(port, baudrate);
    if (SL_IS_FAIL(drv->connect(channel))) {
        qDebug() << "channel unable to connect to LIDAR on" << port;
        return false; // 连接失败
    }
    return true; // 连接成功
}


bool Widget::checkSLAMTECLIDARHealth()
{
    sl_lidar_response_device_health_t healthinfo;
    if (SL_IS_OK(drv->getHealth(healthinfo)) && healthinfo.status == SL_LIDAR_STATUS_OK) {
        return true;
    }
    qDebug() << "LIDAR health check failed or device error.";
    return false;
}

void Widget::startLidarScan()
{
    drv->startScan(0, 1);
}


void Widget::stopLidarScan()
{
    if (drv) {
        drv->stop();
    }
}

void Widget::getData()
{
    QMutexLocker locker(&dataMutex);
    size_t count = nDistance; // 这里的count要不要去掉？？？
    sl_lidar_response_measurement_node_hq_t nodes[count];
    uint32_t rltim_result =drv->grabScanDataHq(nodes, count);
    if (SL_IS_OK(rltim_result)) {
        drv->ascendScanData(nodes, count);
        QVector<LidarPoint> newPoints;
        for (size_t pos = 0; pos < count; ++pos) {
            if(nodes[pos].dist_mm_q2>0){

                float angle = M_PI - nodes[pos].angle_z_q14 * (90.f / 16384.f / (180.0f / M_PI));
                float dist = nodes[pos].dist_mm_q2 / 4000.0f; // 单位是米
                LidarPoint pt;
                if (dist>0){
                    pt.phi = angle;
                    pt.r = dist;
                    pt.x = cos(angle) * dist;
                    pt.y = sin(angle) * dist;
                    pt.signal_strength = nodes[pos].quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT;
                    pt.valid = true;
                    newPoints.append(pt);
//                    LidarPlot->graph(0)->addData(pt.x,pt.y); // temp1
                }
            }

//            LidarPlot->replot(); // temp1

        }
        QMetaObject::invokeMethod(this, [this, newPoints]() {
            for (const auto& pt : newPoints) {
                LidarPlot->graph(0)->addData(pt.x, pt.y);
            }
            LidarPlot->replot();
        }, Qt::QueuedConnection);
    }
}

void Widget::on_lidar_start_clicked()
{
    /* open lidar */
    // 启动雷达扫描
    if (!drv) {  // 确保驱动已经被创建
        qDebug() << "Lidar driver is not initialized!";
        return;
    }
    if (checkSLAMTECLIDARHealth()) {  // 在启动前检查雷达健康状态
        startLidarScan();
        qDebug() << "Lidar scan started successfully.";
        ui->txt_output->setText("Lidar status: running\nmode: free");
    } else {
        qDebug() << "Failed to start lidar scan due to health check failure.";
    }
}


void Widget::on_lidar_stop_clicked()
{
    /* close lidar */
    // 停止雷达扫描
    if (!drv) {  // 确保驱动已经被创建
        qDebug() << "Lidar driver is not initialized!";
        return;
    }
    stopLidarScan();
    ui->txt_output->setText("Lidar status: stopping\nmode: stopping");
    qDebug() << "Lidar scan stopped.";
}



void Widget::on_lidar_mapping_clicked()
{
      /* 设定 地图绘制模式 */
      ui->txt_output->setText("Lidar status: running\nmode: mapping");
      dataTimer->start(100); // 每500毫秒调用一次getData
//      startms(500);
}

void Widget::on_lidar_mapping_clear_clicked()
{
    /* clearing the mapping graph */
    ui->txt_output->setText("Lidar status: running\nmode:  restart mapping");
    // 清空LidarPlot中graph(0)的所有数据点
    LidarPlot->graph(0)->data()->clear();
    // 立即重绘图表以更新图像显示
    LidarPlot->replot();

}
