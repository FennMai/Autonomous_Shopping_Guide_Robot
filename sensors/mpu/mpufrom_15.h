#ifndef __MPU6050_H__
#define __MPU6050_H__

#include <atomic>
#include <functional>

//#include "Sensor.h"
#include "IIC.h"
#include "CppThread.hpp"

#define MPU6050_ADDRESS 0x68
#define GY_271_ADDRESS 0x0D
#define IIC_BUS 1
#define INT_PIN 16  // interrupt pin(BCM)
#define RAD_TO_DEG 57.295779513082320876798154814105   // radium to degree
#define DEG_TO_RAD 0.01745329251994329576923690768489 // degree to radium
#define CALIBRATION_COUNT 5000  

extern std::mutex i2cmtx;

class MPU6050 {

public:

    using CallbackFunction = std::function<void(float[], float[], float[])>;

    MPU6050();

    MPU6050(int customSampleRate, int calibrationTimes);
    
    void getData();

    bool checkNewData();

    void setCallback(CallbackFunction callback);

    static MPU6050* globalInstance;

protected:

    void MPU6050ReadData(char* data);

    void calibrateData();

    void GY271ReadData(char* data);

    void setRangeOfAcce(bool needToExit);

    void setRangeOfGyro(bool needToExit);

    friend void interruptHandler(int GPIO, int level, unsigned int tick);

private:

    CallbackFunction callback_;

    IIC iicMPU6050;
    IIC iicGY271;

    std::atomic<bool> mpu6050_newdata;
    //std::atomic<bool> mpu6050_reading;
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
    std::atomic<float> mx;
    std::atomic<float> my;
    std::atomic<float> mz;

    // static float ax;
    // static float ay;
    // static float az;
    // static float gx;
    // static float gy;
    // static float gz;
    // static float mx;
    // static float my;
    // static float mz;
    
    char MPU6050RawData[14];
    char GY271RawData[6];

    float g_offset[3];
    float a_offset[3];
    float m_offset[3];
};

class MPU6050Thread : public CppThread {

public:

    MPU6050Thread(MPU6050& MPU6050Ins_) : MPU6050Ins(MPU6050Ins_) {};

protected:

    void run() override;

private:

    MPU6050& MPU6050Ins;

};

void interruptHandler(int GPIO, int level, unsigned int tick);

void initializeMPUISR();

#endif