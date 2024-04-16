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


#include "widget.h"
#include "./ui_widget.h"


Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    /***QCustomPlot 连接到qplot***/
    // 初始化 QCustomPlot 对象
    LidarPlot = new QCustomPlot(); // 不需要传递 this，因为后面会添加到布局中
    // 将 QCustomPlot 对象添加到已存在的 QVBoxLayout 中
    ui->qplot->addWidget(LidarPlot); // 使用 ui->qplot，不需要创建新的 QVBoxLayout
    MapPlot = new QCustomPlot(); // 跟上面一样
    ui->mappingplot->addWidget(MapPlot);
    pclplot = new QCustomPlot(); // 跟上面一样
    ui->pclplot->addWidget(pclplot);
    /***调用 setupPlot 函数初始化图表设置***/
    setupPlot();

    // 直接使用多线程，缺点，它是计算好所有的，只是像动画一样展示，我需要的是，实时计算一个数据，实时展示
    //plotThread = std::thread([this] { this->plotData(); });

    /* lidar */
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

    // 连接dataTimer的timeout信号到当前Widget的getData槽函数。
    // 当dataTimer发出timeout信号时（即每次计时器间隔结束时），将自动调用Widget的getData方法
    dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, this, &Widget::getData);
}

Widget::~Widget()
{
    qDebug() << "Destroying Widget, stopping timer.";
    drv->stop();
    delete drv;
    drv = nullptr;
    delete ui; // 删除 UI 对象，不需要单独删除 customPlot，因为它由 Qt 的父子关系自动管理
}


void Widget::setupPlot()
{
    // 配置 LidarPlot 显示范围,地图大小
    LidarPlot->xAxis->setRange(-1, 1);
    LidarPlot->yAxis->setRange(-1, 1);
    // 在这里配置 LidarPlot，设置轴标题
    LidarPlot->xAxis->setLabel("X Axis / m ");
    LidarPlot->yAxis->setLabel("Y Axis / m ");

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
    MapPlot->xAxis->setLabel("X Axis / m ");
    MapPlot->yAxis->setLabel("Y Axis / m ");

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
    pclplot->xAxis->setLabel("X Axis / m ");
    pclplot->yAxis->setLabel("Y Axis / m ");
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
    // 做一个模拟时间变化下，数据在变化的
    rt_location.x = 0;
    rt_location.y =currentIndex;
    // update the car current location output txt
    ui->car_sys_output->setText(QString("Current x: %1\nCurrent y: %2")
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
        pclDetector.clusterLidarData(pathMappingPoints.toStdVector(), clusteredPoints);
        // 数据实时绘图
        // update plotting for LidarPlot & MapPlot
        QMetaObject::invokeMethod(this, [this, LidarCenteredPoints, clusteredPoints]() {
            QVector<float> x, y;
            for (const auto& pt : LidarCenteredPoints) {
                // method 1 -- plot data by matrix
                x.push_back(pt.x);
                y.push_back(pt.y);
                LidarPlot->graph(0)->addData(pt.x, pt.y);
            }
            LidarPlot->replot();

            for (const auto& MapPt : pathMappingPoints) {
                // method 2 -- plot data one by one
                MapPlot->yAxis->setRange(-0.1, 2.5+ currentIndex);
                MapPlot->graph(0)->addData(MapPt.x, MapPt.y);
            }
            MapPlot->replot();

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

void Widget::saveLidarData(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for writing: " << filePath;
        return;
    }

    QTextStream out(&file);
    // 写入文件标题，解释每列数据的含义
    out << "Phi (Radians),R (Meters),X (Meters),Y (Meters),Signal Strength,Valid\n";

    // 遍历 lidarData 中的每个点并写入文件
    for (const LidarPoint &point : pathMappingPoints) {
        out << QString::number(point.phi, 'f', 6) << ","
            << QString::number(point.r, 'f', 6) << ","
            << QString::number(point.x, 'f', 6) << ","
            << QString::number(point.y, 'f', 6) << ","
            << point.signal_strength << ","
            << (point.valid ? "True" : "False") << "\n";
    }

    file.close();
    qDebug() << "Data successfully saved to: " << filePath;
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
        qDebug()<< "Lidar driver is not initialized!";
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
      dataTimer->start(3000);
}

void Widget::on_lidar_mapping_clear_clicked()
{
    /* clearing the mapping graph */
    ui->txt_output->setText("Lidar status: running\nmode:  restart mapping");
    // 清空LidarPlot中graph(0)的所有数据点
    LidarPlot->graph(0)->data()->clear();
    // 立即重绘图表以更新图像显示
    LidarPlot->replot();
    // 同理 MapPlot，这里暂定，感觉不能随时清理map的
//    LidarPlot->graph(0)->data()->clear();
//    LidarPlot->replot();

}

void Widget::on_lidar_mapping_stop_clicked()
{
    /* 设定 关闭地图绘制模式 */
    dataTimer->stop();
    ui->txt_output->setText("Lidar status: running\nmode: stop mapping ");
}




void Widget::on_lidar_mapping_save_clicked()
{
    /* 设定 保存地图数据 */
    dataTimer->stop();
    QString directory = "Map";
    QDir dir(directory);
    if (!dir.exists()) {
        dir.mkpath("."); // 创建目录
    }

    QString filePath = directory + "/pathMappingPoints.dat";
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for writing:" << filePath;
        return;
    }
    saveLidarData("Map/pathMappingPoints.dat");
    QString filename = "Map/Lidar_itself.png"; // 您可以选择不同的格式和文件路径
    LidarPlot->savePng(filename,  1920, 1080); // 参数为文件名、宽度和高度
    MapPlot->savePng("Map/Mapping.png", 1920, 1080);
    ui->txt_output->setText("Lidar status: data saving \nmode: stop mapping ");

}

