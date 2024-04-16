#ifndef MPU6050FILTER_H
#define MPU6050FILTER_H

#include <cmath>
#include <chrono>
#include <pigpio.h>

class MPU6050Filter {
public:
    MPU6050Filter(int i2cBus, int address);
    ~MPU6050Filter();
    void initialize();
    void updateAngles();
    void printData();
    void reset();
    
    double getAngleX() const;  // 新增获取方法
    double getAngleY() const;  // 新增获取方法
    double getAngleZ() const;  // 新增获取方法

private:
    int handle;
    double angleX, angleY, angleZ;
    std::chrono::steady_clock::time_point lastUpdate;
    double getDeltaTime();

    int read_word_2c(int addr);
    double dist(double a, double b);
    double get_y_rotation(double x, double y, double z);
    double get_x_rotation(double x, double y, double z);
};

#endif
