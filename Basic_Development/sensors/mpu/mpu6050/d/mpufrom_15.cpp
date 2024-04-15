#include <iostream>
#include <mutex>

#include "pigpio.h"
#include "mpufrom_15.h"

MPU6050* MPU6050::globalInstance = nullptr;

MPU6050::MPU6050() : MPU6050(200, CALIBRATION_COUNT) {};

MPU6050::MPU6050(int customSampleRate, int calibrationTimes) :
    ax(0.0f),
    ay(0.0f),
    az(0.0f),
    gx(0.0f),
    gy(0.0f),
    gz(0.0f),
    iicMPU6050(MPU6050_ADDRESS),
    a_offset{0.0f, 0.0f, 0.0f}, 
    g_offset{0.0f, 0.0f, 0.0f} {

    offset_count = 0;
    needToExit = false;
    calibrationCount = calibrationTimes;
    globalInstance = this;

    MPU6050RawData[14] = {};

    calibrate_ready.store(false);
    mpu6050_newdata.store(false);

    // 1000/sample_rate_int if DLPF is enabled.
    sampleRate = (1000 / customSampleRate) - 1; 
    std::cout << "Sample rate has been set to " << customSampleRate << std::endl;

    // Wake MPU6050 up.
    char dataString[2] = {0x6B, 0};
    if(iicMPU6050.write(dataString, 2) != 0) {
        std::cerr << "Can't wake MPU6050 up." << std::endl;
    }
    
    // Real sample rate = 1000 / (1+ sampleRate).
    char setSampleRate[2] = { 0x19, sampleRate }; 
    if(iicMPU6050.write(setSampleRate, 2) != 0) {
        std::cerr << "Can't set sample rate of MPU6050." << std::endl;
    } 
    
    // Enable interrupt.
    char enableInt[2] = { 0x38, 0x01 };
    if(iicMPU6050.write(enableInt, 2) != 0) {
        std::cerr << "Can't enable interrupt of MPU6050." << std::endl;
    }

    // Set DLPF.
    char setDLPF[2] = {0x1A, 0x06};
    if(iicMPU6050.write(setDLPF, 2) != 0) {
        std::cerr << "Can't set DLPF of MPU6050." << std::endl;
    }

    // Set full scale range of accelerometer to +-2g
    setRangeOfAcce(needToExit);
    if (needToExit) { return; }
    else { std::cout << "Scale range of accelerometer: +-2g." << std::endl; }

    // Set full scale range of gyroscope to +- 2000 °/s
    setRangeOfGyro(needToExit);
    if (needToExit) { return; }
    else { std::cout << "Scale range of gyroscope: +- 2000°/s." << std::endl; }
}

void MPU6050::MPU6050ReadData(char* data) {
    char MPU6050StartAddr = 0x3B;
    if(iicMPU6050.read(data, 14, MPU6050StartAddr) != 0) {
        std::cerr << "Can't read data of MPU6050." << std::endl;
    }
}

void MPU6050::setRangeOfAcce(bool needToExit) {
    char dataAcce[2] = {0x1C, 0 << 3};
    iicMPU6050.write(dataAcce, 2);
}

void MPU6050::setRangeOfGyro(bool needToExit) {
    char dataGyro[2] = {0x1B, 0x18};
    iicMPU6050.write(dataGyro, 2);
}

void MPU6050::setCallback(CallbackFunction callback) {
    callback_ = callback;
}

void MPU6050::getData() {
    float a[3] = {};
    float g[3] = {};
    mpu6050_newdata.store(false);

    a[0] = ((ax.load() - a_offset[0]) / 16384.0f)*9.818f;
    a[1] = ((ay.load() - a_offset[1]) / 16384.0f)*9.818f;
    a[2] = ((az.load() - a_offset[2]) / 16384.0f)*9.818f + 9.818f;

    g[0] = (gx.load() - g_offset[0]) / 16.384f * DEG_TO_RAD;
    g[1] = (gy.load() - g_offset[1]) / 16.384f * DEG_TO_RAD;
    g[2] = (gz.load() - g_offset[2]) / 16.384f * DEG_TO_RAD;

    if(callback_) {
        callback_(a, g);
    }
}

void MPU6050::calibrateData() {
    g_offset[0] += gx.load();
    g_offset[1] += gy.load();
    g_offset[2] += gz.load();

    a_offset[0] += ax.load();
    a_offset[1] += ay.load();
    a_offset[2] += az.load();

    std::cout << "Calibrating! " << (calibrationCount - offset_count) << " times left." << std::endl;

    offset_count++;

    if (offset_count > calibrationCount) {
        g_offset[0] /= offset_count;
        g_offset[1] /= offset_count;
        g_offset[2] /= offset_count;

        a_offset[0] /= offset_count;
        a_offset[1] /= offset_count;
        a_offset[2] /= offset_count;

        offset_count = 0;
        calibrate_ready.store(true);
    }
}
