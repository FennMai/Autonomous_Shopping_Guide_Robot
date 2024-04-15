#ifndef __MPU6050_H__
#define __MPU6050_H__

#include <atomic>
#include <functional>
#include "i2c.h"
#include "CppThread.hpp"

// 定义MPU6050的I2C地址和一些转换常数
#define MPU6050_ADDRESS 0x68
#define IIC_BUS 1
#define INT_PIN 16  // 中断引脚编号(BCM模式)
#define RAD_TO_DEG 57.295779513082320876798154814105   // 弧度转度
#define DEG_TO_RAD 0.01745329251994329576923690768489 // 度转弧度
#define CALIBRATION_COUNT 5000  // 校准次数

extern std::mutex i2cmtx;  // 定义一个用于I2C通信的互斥锁

class MPU6050 {

public:
    using CallbackFunction = std::function<void(float[], float[])>;

    MPU6050();
    MPU6050(int customSampleRate, int calibrationTimes);

    void getData();
    bool checkNewData();
    void setCallback(CallbackFunction callback);

    static MPU6050* globalInstance; // 全局实例的静态指针

protected:
    void MPU6050ReadData(char* data);
    void calibrateData();
    void setRangeOfAcce(bool needToExit);
    void setRangeOfGyro(bool needToExit);

    friend void interruptHandler(int GPIO, int level, unsigned int tick); // 允许中断处理函数访问私有成员

private:
    CallbackFunction callback_;

    IIC iicMPU6050;  // MPU6050的I2C接口对象

    std::atomic<bool> mpu6050_newdata;
    std::atomic<bool> calibrate_ready;
    bool needToExit;
    int offset_count;

    int calibrationCount;

    char sampleRate;

    std::atomic<float> ax;
    std::atomic<float> ay;
    std::atomic<float> az;
    std::atomic<float> gx;
    std::atomic<float> gy;
    std::atomic<float> gz;

    char MPU6050RawData[14];

    float g_offset[3];
    float a_offset[3];
};

class MPU6050Thread : public CppThread {

public:
    MPU6050Thread(MPU6050& MPU6050Ins_) : MPU6050Ins(MPU6050Ins_) {}

protected:
    void run() override;

private:
    MPU6050& MPU6050Ins;
};

void interruptHandler(int GPIO, int level, unsigned int tick);
void initializeMPUISR();  // 初始化中断服务例程

#endif // __MPU6050_H__
