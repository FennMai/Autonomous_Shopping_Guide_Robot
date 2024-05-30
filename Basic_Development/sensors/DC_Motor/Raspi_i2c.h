#ifndef Raspi_i2c_H  // 如果没有定义Raspi_i2c_H宏
#define Raspi_i2c_H  // 定义Raspi_i2c_H宏

#include <math.h>     // 包含数学库头文件
#include <stdint.h>   // 包含标准整数类型定义头文件
#include <stdio.h>    // 包含标准输入输出头文件
#include <pigpio.h>   // 包含pigpio库头文件，提供对树莓派GPIO的控制

// 定义一个名为Raspi_I2C的类
class Raspi_I2C {
 public:
  // 构造函数，初始化Raspi_I2C对象
  Raspi_I2C();
  
  // 析构函数，确保资源被正确释放
  ~Raspi_I2C();
  
  // 初始化I2C设备，设置I2C地址
  void init(uint8_t address);
  
  // 向指定寄存器写入8位数据
  void WriteReg8(uint8_t reg, uint8_t value);
  
  // 向指定寄存器写入16位数据
  void WriteReg16(uint8_t reg, uint16_t value);
  
  // 写入单个8位数据
  void WriteBit8(uint8_t value);
  
  // 从指定寄存器读取8位数据
  uint8_t ReadReg8(uint8_t reg);
  
  // 从指定寄存器读取16位数据
  uint16_t ReadReg16(uint8_t reg);
  
  // 读取单个8位数据
  uint8_t ReadBit8();
  
  // 写入多个字节数据
  uint32_t Write(uint8_t* data, uint32_t size);

 private:
  int handle_; 
