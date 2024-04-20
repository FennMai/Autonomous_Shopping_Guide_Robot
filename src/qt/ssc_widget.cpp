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
 * basic 2.5, date:15/04/2024:
 * basic lidar + car control in real time
 *
 * basic 3.0, date:17/04/2024:
 * basic lidar + car control + voic in real time
 *
 * basic 3.1, date:18/04/2024:
 * basic lidar + car control + voic test 1
 *
 * basic 3.2, date:20/04/2024:
 * (1)basic lidar + car control + voic test 2
 * (2)add open gif
 *
 * basic 3.2, date:20/04/2024:
 * (1)basic lidar + car control + voic test 3, fix the bug
 * (2)add open gif
 **/

#include "ssc_widget.h"
#include "./ui_ssc_widget.h"
#include "CarControl.hpp"
#include <QLabel>
#include <QMovie>

SSC_Widget::SSC_Widget(QWidget *parent): QWidget(parent), ui(new Ui::SSC_Widget),soundManager(new SoundManager(1, 0x64))
{
    ui->setupUi(this);
    /*-----setting GIF when start the qt-----*/
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

    connect(animationMovie, &QMovie::frameChanged, this, &SSC_Widget::checkLastFrame);

    /*-----初始化 QCustomPlot 对象-----*/
    LidarPlot = new QCustomPlot();
    ui->LidarPlot->addWidget(LidarPlot);
    MapPlot = new QCustomPlot();
    ui->MapPlot->addWidget(MapPlot);
    pclplot = new QCustomPlot();
    ui->pclplot->addWidget(pclplot);
    setupPlot();

    /*-----lidar-----*/
    // 创建激光雷达驱动实例
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
    try {
            soundManager->startCheckingCMDID();  // 开始检查CMDID
        } catch (const std::exception& e) {
            ui->txt_output->setText(QString::fromStdString(e.what()));
            return;
        }
    /*-----Timer setting-----*/
    dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, this, &SSC_Widget::getData);

    modeTimer = new QTimer(this);
    connect(modeTimer, &QTimer::timeout, this, &SSC_Widget::vocie_update);
    modeTimer->start(1000);

    /*-----car initialize-----*/
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
//    delete dataTimer;
//    delete modeTimer;
    delete soundManager;
    delete animationLabel;
    delete animationMovie;
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

    // 绘图风格showFullScreen
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

void SSC_Widget::vocie_update(){
    // 从共享内存读取CMDID，并更新UI
    soundManager->updateOutputText([this](int CMDID) {  // 确保 CMDID 作为参数传入
            QMetaObject::invokeMethod(this, [this, CMDID]() {  // 正确捕获 CMDID
                cmdid = CMDID;  // 更新类成员变量
                ui->mode_output->setText("当前状态是：CMDID " + QString::number(CMDID));
                processCommand(cmdid);
            }, Qt::QueuedConnection);
        });
}

void SSC_Widget::processCommand(int cmdid) {
    // receive the voice command to control the system
    switch(cmdid) {
        case 5:
            ui->mode_output->setText("Sure! 5! Start Lidar");
            startLidarScan();
            break;
        case 6:
            ui->mode_output->setText("Sure! 6! Start mapping");
            lidarmapping();

            break;
        case 7:
            ui->mode_output->setText("Sure! 7! start shopping");
            break;
        case 8:
            ui->mode_output->setText("Sure! 8! moving to cart");
            ui->pushButton_4->click(); // car driving
            break;
        case 9:
            ui->mode_output->setText("Sure! 8! find apple");
            break;

        case 10:
            ui->mode_output->setText("Sure! 8! moving to cart");
            break;
        case 13:
            // 当 CMDID 为 9 时关闭应用程序
            qDebug() << "Received shutdown command. Closing application.";
            QCoreApplication::quit();
            break;
        default:
            // 默认处理
            break;
    }
}


void SSC_Widget::getData()
{
    // get from car
    rt_location.x = carControl->getXPosition();
    rt_location.y =carControl->getYPosition();
    // update the car current location output txt
    ui->car_sys_output->setText(QString("Current x: %1\nCurrent y: %2\n")
                                .arg(rt_location.x, 0, 'f', 2)
                                .arg(rt_location.y, 0, 'f', 2));

   // realtime
    QVector<LidarPoint> wallPoints; // 用于存储识别为墙的点
    QVector<LidarPoint> obstaclePoints; // 用于存储识别为障碍物的点

    currentIndex ++;
    QMutexLocker locker(&dataMutex);
    size_t count = nDistance; // ount
    sl_lidar_response_measurement_node_hq_t nodes[count];
    uint32_t rltim_result =drv->grabScanDataHq(nodes, count);
    if (SL_IS_OK(rltim_result)) {
        drv->ascendScanData(nodes, count);
        // set a temp nodes vector
        QVector<LidarPoint> LidarCenteredPoints;
        QVector<LidarPoint> carCenteredPoints;
        int obstacleCount = 0;
        const float obstacleDetectionRange = 0.10;// 前方0.5米检测范围
        const int obstacleThreshold = 10; // 阈值为10个点
        // data pre-filter
        for (size_t pos = 0; pos < count; ++pos) {
            // length:240 width : 130
            // 下面if 作为取值筛选，只需要距离为[0,12]米之间的
            // 条件语句放在同一个 if 语句中来避免嵌套，这样可以提高代码可读性和执行效率。
            if(nodes[pos].dist_mm_q2>lidar_cal_range_low && nodes[pos].dist_mm_q2 < lidar_cal_range_upper){

                float angle = nodes[pos].angle_z_q14 * (90.f / 16384.f / (180.0f / M_PI));
                //float angle = nodes[pos].angle_z_q14 / 16384.0f * 360.0f * (M_PI / 180.0f);
                float dist = nodes[pos].dist_mm_q2 / 4000.0f; // 单位是米
                LidarPoint CenPt;
                LidarPoint MapPt;
                LidarPoint wallPt;
                LidarPoint carPt;
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

                    if (dist <= obstacleDetectionRange) {
                          if (CenPt.x >= - 0.08 && CenPt.x <=  0.08) { // 检测前方宽8厘米的区域
                              obstacleCount++;
                          }
                      }
                    // 保存的是最后建图有效的数据点采集
                    // cmdid =6 mapping, cmdid = 7 shopping
                    if (cmdid == 6){
                            MapPt.phi = angle;
                            MapPt.r = dist;
                            MapPt.x = rt_location.x + sin(angle) * dist;
                            MapPt.y = rt_location.y + cos(angle) * dist;
                            MapPt.signal_strength = nodes[pos].quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT;
                            MapPt.valid = true;
                            pathMappingPoints.append(MapPt); // all data
                }
                    if (cmdid == 7){
                        MapPt.phi = angle;
                        MapPt.r = dist;
                        MapPt.x = rt_location.x;// 它本身，就是坐标点
                        MapPt.y = rt_location.y;
                        MapPt.signal_strength = nodes[pos].quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT;
                        MapPt.valid = true;

                        carCenteredPoints.append(MapPt);
                        QCPScatterStyle scatterStyle;
                        scatterStyle.setShape(QCPScatterStyle::ssSquare);
//                        scatterStyle.setSize(3); // 设置点的大小
//                        scatterStyle.setPen(QPen(Qt::red)); // 设置点边界的颜色
//                        scatterStyle.setBrush(Qt::yellow); // 设置点的填充颜色
                        MapPlot->graph(0)->setScatterStyle(scatterStyle);
                        MapPlot->graph(0)->addData(MapPt.x, MapPt.y);
                    }


                }
            }
        }
        // detect
        if (obstacleCount > obstacleThreshold) {
             qDebug() << "Obstacle detected in front of the car!";
             ui->car_sys_output->setText("Obstacle detected in front of the car!");
             std::cout << "Obstacle detected in front of the car\n";
             ob_detect_050 = true;
         } else {
             qDebug() << "No obstacle detected in front of the car!";
             std::cout << "No obstacle detected in front of the car\n";
             ob_detect_050 = false;
         }
        // detect 完毕，运动部分
//         car_drive(ob_detect_050);

        // PCL
//        std::vector<std::vector<LidarPoint>> clusteredPoints;
//        pclDetector.clusterLidarData(pathMappingPoints.toStdVector(), clusteredPoints);
//        std::vector<LidarPoint> stdVector(pathMappingPoints.begin(), pathMappingPoints.end());
//        pclDetector.clusterLidarData(stdVector, clusteredPoints);
        // plotting
        // update plotting for LidarPlot & MapPlot
        QMetaObject::invokeMethod(this, [this, LidarCenteredPoints, carCenteredPoints]() {
            // LidarPlot
            QVector<float> x, y;
            LidarPlot->graph(0)->data()->clear();
            LidarPlot->replot();
//            int count = 0;
            for (const auto& pt : LidarCenteredPoints) {
//                // method 1 -- plot data by matrix
//                x.push_back(pt.x);
//                y.push_back(pt.y);
                LidarPlot->graph(0)->addData(pt.x, pt.y);
            }
            LidarPlot->replot();
            // MapPlot
            if(cmdid == 6){
                for (const auto& MapPt : pathMappingPoints) {
                    // method 2 -- plot data one by one
                    MapPlot->graph(0)->addData(MapPt.x, MapPt.y);
                    // detect ob
    //                detect_ob();
                }
                MapPlot->replot();
            }
            if(cmdid == 7){
                for (const auto& MapPt : carCenteredPoints) {
                    // method 2 -- plot data one by one
                    MapPlot->graph(0)->addData(MapPt.x, MapPt.y);
                }
                MapPlot->replot();
            }

            // pclplot
//            for (const auto& cluster : clusteredPoints) {
//                for (const auto& pt : cluster) {
//                    pclplot->yAxis->setRange(-0.1, 2.5+ currentIndex);
//                    pclplot->graph(0)->addData(pt.x, pt.y);
//                }
//            }
//            pclplot->replot();
        }, Qt::QueuedConnection);
    }
}


void SSC_Widget::car_drive(bool ob_detect_050){

    if (ob_detect_050) {
        carControl->moveBackward(150, [this]() {
            std::cout << "Moved 150 cm backward.\n";
            // 这里可以根据需要添加更多的逻辑，比如再次前进或转向
        });
    } else {
        carControl->moveForward(150, [this]() {
            std::cout << "Moved 150 cm forward.\n";
            // 根据前进后的数据再次评估，可能需要后退
        });
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
    dataTimer->stop(); // stop mapping
    stopLidarScan();
    ui->txt_output->setText("Lidar status: stopping\nmode: stopping");
    qDebug() << "Lidar scan stopped.";
}


void SSC_Widget::on_pushButton_3_clicked()
{
    /* 设定 地图绘制模式 */
    ui->txt_output->setText("Lidar status: running");
    // open dataTimer
    dataTimer->start(1000);
}


void SSC_Widget::on_pushButton_4_clicked()
{
    //boundary of the shop
    carControl->moveForward(50, []() {
        std::cout << "Moved 50 cm forward.\n";
    });
//    carControl->moveForward(50, []() {
//        std::cout << "Moved 155 cm forward.\n";
//    });
//    carControl->moveBackward(50, []() {
//        std::cout << "Turned right by 90 degrees.\n";
//    });

//    carControl->moveForward(35, []() {
//        std::cout << "Moved 35 cm forward.\n";
//    });

//    carControl->turnRight(90, []() {
//        std::cout << "Turned right by 90 degrees.\n";
//    });

//    carControl->moveForward(133, []() {
//        std::cout << "Moved 133 cm forward.\n";
//    });

//    carControl->turnRight(90, []() {
//        std::cout << "Turned right by 90 degrees.\n";
//    });

//    carControl->moveForward(57, []() {
//        std::cout << "Moved 57 cm forward.\n";
//    });

}

void SSC_Widget::checkLastFrame(int frameNumber) {
   if (frameNumber == animationMovie->frameCount() - 1) {
       // end the gif
       animationMovie->stop();
       animationLabel->hide();
       // change the window
       showNormal();
       setWindowFlags(Qt::Window);
       show();
   }
}

