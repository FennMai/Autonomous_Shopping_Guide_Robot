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
 *
 **/


#include "ssc_widget.h"
#include "./ui_ssc_widget.h"
#include "CarControl.hpp"

SSC_Widget::SSC_Widget(QWidget *parent): QWidget(parent), ui(new Ui::SSC_Widget)
{
    ui->setupUi(this);

    // 初始化 QCustomPlot 对象
    LidarPlot = new QCustomPlot();
    ui->LidarPlot->addWidget(LidarPlot);
    MapPlot = new QCustomPlot();
    ui->MapPlot->addWidget(MapPlot);
    pclplot = new QCustomPlot();
    ui->pclplot->addWidget(pclplot);
    setupPlot();

    /* lidar */
    // 创建激光雷达驱动实例
//    drv = *sl::createLidarDriver();
    drv = *sl::createLidarDriver();
    if (!drv) {
        qDebug() << "Insufficient memory, exit";
        exit(-1);
    }
    if (!initializeLidar(netport, baudrate) || !checkSLAMTECLIDARHealth()) {
        delete drv;
        drv = nullptr;
        qDebug() << "Lidar initialization failed";
        exit(-1);
    }

    dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, this, &SSC_Widget::getData);

    // car
    carControl = CarControl::getInstance();
    carControl->initialize();
}

SSC_Widget::~SSC_Widget()
{
    qDebug() << "Destroying Widget, stopping timer.";
    drv->stop();
    delete drv;
    drv = nullptr;
    delete ui;
    delete LidarPlot;
    delete MapPlot;
    delete pclplot;
    delete dataTimer;

}

void SSC_Widget::setupPlot()
{
    // 配置 LidarPlot 显示范围,地图大小
    LidarPlot->xAxis->setRange(-1, 1);
    LidarPlot->yAxis->setRange(-1, 1);
    // 在这里配置 LidarPlot，设置轴标题
//    LidarPlot->xAxis->setLabel("X Axis / m ");
//    LidarPlot->yAxis->setLabel("Y Axis / m ");

    // 创建一个新的图形用于绘制数据
    LidarPlot->addGraph();
    LidarPlot->graph(0)->setName("Lidar Dynamic Data");

    // 绘图风格
    LidarPlot->graph(0)->setLineStyle(QCPGraph::lsNone); // 不使用线连接点
    QCPScatterStyle scatterStyle;
    // 设置形状为正方形
    // ssCircle 圆形
    scatterStyle.setShape(QCPScatterStyle::ssSquare);
    scatterStyle.setSize(3); // 设置点的大小
    scatterStyle.setPen(QPen(Qt::red)); // 设置点边界的颜色
    scatterStyle.setBrush(Qt::yellow); // 设置点的填充颜色
    LidarPlot->graph(0)->setScatterStyle(scatterStyle);

    // MapPlot
    // 配置 LidarPlot 显示范围,地图大小
    MapPlot->xAxis->setRange(-0.1, 1.4);
    MapPlot->yAxis->setRange(-0.1, 2.5);
    // 在这里配置 LidarPlot，设置轴标题
//    MapPlot->xAxis->setLabel("X Axis / m ");
//    MapPlot->yAxis->setLabel("Y Axis / m ");

    // 创建一个新的图形用于绘制数据
    MapPlot->addGraph();
    MapPlot->graph(0)->setName("Lidar Dynamic Data");

    // 绘图风格
    MapPlot->graph(0)->setLineStyle(QCPGraph::lsNone); // 不使用线连接点
    //QCPScatterStyle scatterStyle;
    // 设置形状为正方形
    // ssCircle 圆形
    scatterStyle.setShape(QCPScatterStyle::ssSquare);
    scatterStyle.setSize(3); // 设置点的大小
    scatterStyle.setPen(QPen(Qt::red)); // 设置点边界的颜色
    scatterStyle.setBrush(Qt::yellow); // 设置点的填充颜色
    MapPlot->graph(0)->setScatterStyle(scatterStyle);

    // 配置 LidarPlot 显示范围,地图大小
    pclplot->xAxis->setRange(-1, 1);
    pclplot->yAxis->setRange(-1, 1);
    // 在这里配置 LidarPlot，设置轴标题
//    pclplot->xAxis->setLabel("X Axis / m ");
//    pclplot->yAxis->setLabel("Y Axis / m ");
    // 创建一个新的图形用于绘制数据
    pclplot->addGraph();
    pclplot->graph(0)->setName("Lidar Dynamic Data");

    // 绘图风格
    pclplot->graph(0)->setLineStyle(QCPGraph::lsNone); // 不使用线连接点
    //QCPScatterStyle scatterStyle;
    // 设置形状为正方形
    // ssCircle 圆形
    scatterStyle.setShape(QCPScatterStyle::ssSquare);
    scatterStyle.setSize(3); // 设置点的大小
    scatterStyle.setPen(QPen(Qt::red)); // 设置点边界的颜色
    scatterStyle.setBrush(Qt::yellow); // 设置点的填充颜色
    pclplot->graph(0)->setScatterStyle(scatterStyle);
}

bool SSC_Widget::initializeLidar(const char* port, sl_u32 baudrate)
{
    sl::IChannel* channel = *sl::createSerialPortChannel(port, baudrate);
    ui->txt_output->setText("Lidar status: initialize...\n");
    if (SL_IS_FAIL(drv->connect(channel))) {
        qDebug() << "channel unable to connect to LIDAR on" << port;
        return false;
    }
    return true;
}

bool SSC_Widget::checkSLAMTECLIDARHealth()
{
    sl_lidar_response_device_health_t healthinfo;
    if (SL_IS_OK(drv->getHealth(healthinfo)) && healthinfo.status == SL_LIDAR_STATUS_OK) {
        ui->txt_output->setText("Lidar status: check health...\n");
        return true;
    }
    qDebug() << "LIDAR health check failed or device error.";
    return false;
}

void SSC_Widget::startLidarScan()
{
    /* open lidar */
    // 启动雷达扫描
    if (!drv) {  // 确保驱动已经被创建
        qDebug() << "Lidar driver is not initialized!";
        return;
    }
    if (checkSLAMTECLIDARHealth()) {  // 在启动前检查雷达健康状态
        drv->startScan(0, 1);
        qDebug() << "Lidar scan started successfully.";
        ui->txt_output->setText("Lidar status: running\n");
    } else {
        qDebug() << "Failed to start lidar scan due to health check failure.";
    }
}


void SSC_Widget::stopLidarScan()
{
    if (drv) {
        /* close lidar */
        // 停止雷达扫描
        if (!drv) {  // 确保驱动已经被创建
            qDebug()<< "Lidar driver is not initialized!";
            return;
        }
        drv->stop();
        ui->txt_output->setText("Lidar status: stopping");
        qDebug() << "Lidar scan stopped.";
    }
}

void SSC_Widget::lidarmapping()
{
      /* 设定 地图绘制模式 */
      ui->txt_output->setText("Lidar status: running\nmode: mapping");
      dataTimer->start(1000);
}


void SSC_Widget::getData()
{
    // simulate

    rt_location.x = carControl->getXPosition();
    rt_location.y =carControl->getYPosition();
    // update the car current location output txt
    ui->car_sys_output->setText(QString("Current x: %1\nCurrent y: %2\n")
                                .arg(rt_location.x, 0, 'f', 2)
                                .arg(rt_location.y, 0, 'f', 2));
    currentIndex ++;
    QMutexLocker locker(&dataMutex);
    size_t count = nDistance; // 这里的count要不要去掉？？？
    sl_lidar_response_measurement_node_hq_t nodes[count];
    // 数据抓取
    uint32_t rltim_result =drv->grabScanDataHq(nodes, count);
    if (SL_IS_OK(rltim_result)) {
        drv->ascendScanData(nodes, count);
        // set a temp nodes vector
        QVector<LidarPoint> LidarCenteredPoints;
        // 数据预处理
        for (size_t pos = 0; pos < count; ++pos) {
            // 下面if 作为取值筛选，只需要距离为[0,1]米之间的
            // 条件语句放在同一个 if 语句中来避免嵌套，这样可以提高代码可读性和执行效率。
            if(nodes[pos].dist_mm_q2>lidar_cal_range_low && nodes[pos].dist_mm_q2 < lidar_cal_range_upper){

                float angle = nodes[pos].angle_z_q14 * (90.f / 16384.f / (180.0f / M_PI));
                //float angle = nodes[pos].angle_z_q14 / 16384.0f * 360.0f * (M_PI / 180.0f);
                float dist = nodes[pos].dist_mm_q2 / 4000.0f; // 单位是米
                LidarPoint CenPt;
                LidarPoint MapPt;

                if ((nodes[pos].quality>> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT)!=0 &&
                        ((angle >= 0 && angle <= M_PI / 2) || (angle >= 3 * M_PI / 2 && angle <= 2 * M_PI))){
                    // 以自我为中心的数据点采集
                    CenPt.phi = angle;
                    CenPt.r = dist;
                    CenPt.x = sin(angle) * dist;
                    CenPt.y = cos(angle) * dist;
                    CenPt.signal_strength = nodes[pos].quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT;
                    CenPt.valid = true;
                    LidarCenteredPoints.append(CenPt); // temp data

                    // 保存的是最后建图有效的数据点采集
                    MapPt.phi = angle;
                    MapPt.r = dist;
                    MapPt.x = rt_location.x + sin(angle) * dist;
                    MapPt.y = rt_location.y + cos(angle) * dist;
                    MapPt.signal_strength = nodes[pos].quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT;
                    MapPt.valid = true;
                    pathMappingPoints.append(MapPt); // all data
                }
            }
        }
        // 聚类处理
        std::vector<std::vector<LidarPoint>> clusteredPoints;
//        pclDetector.clusterLidarData(pathMappingPoints.toStdVector(), clusteredPoints);
        std::vector<LidarPoint> stdVector(pathMappingPoints.begin(), pathMappingPoints.end());
        pclDetector.clusterLidarData(stdVector, clusteredPoints);
        // 数据实时绘图
        // update plotting for LidarPlot & MapPlot
        QMetaObject::invokeMethod(this, [this, LidarCenteredPoints, clusteredPoints]() {
            // LidarPlot
            QVector<float> x, y;
            LidarPlot->graph(0)->data()->clear();
            LidarPlot->replot();
            for (const auto& pt : LidarCenteredPoints) {
                // method 1 -- plot data by matrix
                x.push_back(pt.x);
                y.push_back(pt.y);
                LidarPlot->graph(0)->addData(pt.x, pt.y);
            }
            LidarPlot->replot();
            // MapPlot
            for (const auto& MapPt : pathMappingPoints) {
                // method 2 -- plot data one by one
                MapPlot->yAxis->setRange(-0.1, 2.5+ currentIndex);
                MapPlot->graph(0)->addData(MapPt.x, MapPt.y);
            }
            MapPlot->replot();
            // pclplot
            for (const auto& cluster : clusteredPoints) {
                for (const auto& pt : cluster) {
                    pclplot->yAxis->setRange(-0.1, 2.5+ currentIndex);
                    pclplot->graph(0)->addData(pt.x, pt.y);
                }
            }
            pclplot->replot();
        }, Qt::QueuedConnection);
    }
}

void SSC_Widget::on_pushButton_clicked()
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


void SSC_Widget::on_pushButton_2_clicked()
{
    /* close lidar */
    // 停止雷达扫描
    if (!drv) {  // 确保驱动已经被创建
        qDebug()<< "Lidar driver is not initialized!";
        return;
    }
    stopLidarScan();
    ui->txt_output->setText("Lidar status: stopping\nmode: stopping");
    qDebug() << "Lidar scan stopped.";
}


void SSC_Widget::on_pushButton_3_clicked()
{
    /* 设定 地图绘制模式 */
    ui->txt_output->setText("Lidar status: running\nmode: mapping");
    dataTimer->start(3000);
}


void SSC_Widget::on_pushButton_4_clicked()
{
    //boundary of the shop
    carControl->moveForward(160, []() {
        std::cout << "Moved 160 cm forward.\n";
    });

    carControl->turnRight(90, []() {
        std::cout << "Turned right by 90 degrees.\n";
    });

    carControl->moveForward(35, []() {
        std::cout << "Moved 35 cm forward.\n";
    });

    carControl->turnRight(87, []() {
        std::cout << "Turned right by 90 degrees.\n";
    });

    carControl->moveForward(130, []() {
        std::cout << "Moved 130 cm forward.\n";
    });

    carControl->turnRight(85, []() {
        std::cout << "Turned right by 90 degrees.\n";
    });

    carControl->moveForward(45, []() {
        std::cout << "Moved 45 cm forward.\n";
    });

/*  carControl->moveForward(100, []() {
        std::cout << "Moved 100 cm forward.\n";
    });

    carControl->turnRight(90, []() {
        std::cout << "Turned right by 90 degrees.\n";
    });

    carControl->moveForward(100, []() {
        std::cout << "Moved 100 cm forward.\n";
    });

    carControl->turnRight(90, []() {
        std::cout << "Turned right by 90 degrees.\n";
    });

    carControl->moveForward(100, []() {
        std::cout << "Moved 100 cm forward.\n";
    });

    carControl->turnRight(90, []() {
        std::cout << "Turned right by 90 degrees.\n";
    });

    carControl->moveForward(100, []() {
        std::cout << "Moved 100 cm forward.\n";
    });
*/

}

