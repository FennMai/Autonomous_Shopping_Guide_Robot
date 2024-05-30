#ifndef _Emakefun_MotorDriver_H  // 如果没有定义宏 _Emakefun_MotorDriver_H
#define _Emakefun_MotorDriver_H  // 定义宏 _Emakefun_MotorDriver_H，防止头文件重复包含

#include <pigpio.h>    // 包含pigpio库头文件，提供对树莓派GPIO的控制
#include "Raspi_i2c.h" // 包含自定义的Raspi_i2c.h头文件，提供对Raspi_I2C类的声明和定义

// 定义一些PCA9685芯片的寄存器地址和常量
#define PCA9685_SUBADR1 0x2  // PCA9685芯片的子地址1
#define PCA9685_SUBADR2 0x3  // PCA9685芯片的子地址2
#define PCA9685_SUBADR3 0x4  // PCA9685芯片的子地址3

#define PCA9685_MODE1 0x00    // PCA9685芯片的模式寄存器1
#define PCA9685_PRESCALE 0xFE // PCA9685芯片的预分频寄存器

#define LED0_ON_L 0x6  // 第0个LED的打开时间低位
#define LED0_ON_H 0x7  // 第0个LED的打开时间高位
#define LED0_OFF_L 0x8 // 第0个LED的关闭时间低位
#define LED0_OFF_H 0x9 // 第0个LED的关闭时间高位

#define ALLLED_ON_L 0xFA  // 所有LED的打开时间低位
#define ALLLED_ON_H 0xFB  // 所有LED的打开时间高位
#define ALLLED_OFF_L 0xFC // 所有LED的关闭时间低位
#define ALLLED_OFF_H 0xFD // 所有LED的关闭时间高位

// 定义一个名为Emakefun_MotorDriver的类，继承自Raspi_I2C类
class Emakefun_MotorDriver : public Raspi_I2C {
 public:
  // 构造函数，默认I2C地址为0x60
  Emakefun_MotorDriver(uint8_t addr = 0x60);
  
  // 初始化方法
  void begin(void);
  
  // 复位PCA9685芯片
  void reset(void);
  
  // 设置PWM频率
  void setPWMFreq(float freq);
  
  // 设置PWM的打开和关闭时间
  void setPWM(uint8_t num, uint16_t on, uint16_t off);

 private:
  uint8_t _i2caddr;  // I2C设备地址
  
  // 从指定地址读取8位数据
  uint8_t read8(uint8_t addr);
  
  // 向指定地址写入8位数据
  void write8(uint8_t addr, uint8_t d);
};

#endif  // 结束预处理指令条件编译
