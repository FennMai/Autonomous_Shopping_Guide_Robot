/**
 * author : ZMai
 * last modified date: 10/04/2024
 * last commit: 
 * driving files: 
 * 
 * basic 1.1, date:10/04/2024:
 * basic from the simple_ultra.cpp, the change is the env --- from makefile to camkelist.txt
 * 
 * basic 1.2, date:10/04/2024:
 * Refining code
 **/

#include <iostream>
#include <unistd.h>

#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "sdk/include/sl_lidar.h" 
#include "sdk/include/sl_lidar_driver.h"
#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

using namespace sl;

// 全局变量用于在SIGINT信号处理时修改状态
bool ctrl_c_pressed = false;

// SIGINT信号处理函数，设置全局变量以退出主循环
void ctrlc(int sig) {
    ctrl_c_pressed = true;
}

// 延时函数，直接使用usleep
void delay(sl_word_size_t ms) {
    usleep(ms * 1000);
}

// 初始化激光雷达
bool initializeLidar(ILidarDriver* drv, const char* port, sl_u32 baudrate) {
    // 假设drv已经被成功创建
    // 尝试连接到激光雷达
    IChannel* channel = *createSerialPortChannel(port, baudrate);
    if (SL_IS_OK(drv->connect(channel))) {
        return true;
    }
    std::cerr << "Unable to connect to LIDAR on " << port << std::endl;
    return false;
}

// 检查激光雷达健康状态
bool checkSLAMTECLIDARHealth(ILidarDriver* drv) {
    sl_lidar_response_device_health_t healthinfo;
    auto op_result = drv->getHealth(healthinfo);
    if (SL_IS_OK(op_result) && healthinfo.status == SL_LIDAR_STATUS_OK) {
        return true;
    }
    std::cerr << "LIDAR health check failed or device error." << std::endl;
    return false;
}

// 获取并打印激光雷达扫描数据
void fetchAndPrintScanData(ILidarDriver* drv) {
    sl_lidar_response_measurement_node_hq_t nodes[8192];
    size_t count = _countof(nodes);
    auto op_result = drv->grabScanDataHq(nodes, count);
    if (SL_IS_OK(op_result)) {
        drv->ascendScanData(nodes, count);
        for (size_t pos = 0; pos < count; ++pos) {
            std::cout << "Theta: " << (nodes[pos].angle_z_q14 * 90.0f) / 16384.0f
                      << " Dist: " << nodes[pos].dist_mm_q2 / 4.0f
                      << " Quality: " << (nodes[pos].quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT)
                      << std::endl;
        }
    }
}

int main(int argc, const char* argv[]) {
    signal(SIGINT, ctrlc); // 设置信号处理

    // 创建激光雷达驱动实例
    ILidarDriver* drv = *createLidarDriver();
    if (!drv) {
        std::cerr << "Insufficient memory, exit\n";
        return -1;
    }

    // 硬编码串口和波特率，应根据实际情况修改或通过参数传递
    if (!initializeLidar(drv, "/dev/ttyUSB0", 460800) || !checkSLAMTECLIDARHealth(drv)) {
        delete drv; // 清理资源
        return -1;
    }

    
    drv->startScan(0, 1); // 开始扫描
    while (!ctrl_c_pressed) {
        fetchAndPrintScanData(drv);
        delay(100); // 稍微延时，防止过快循环
    }
    drv->stop(); // 停止扫描

    // 清理和退出
    delete drv;
    return 0;
}
