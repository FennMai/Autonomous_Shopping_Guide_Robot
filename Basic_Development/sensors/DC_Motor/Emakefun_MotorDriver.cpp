#include "Emakefun_MotorDriver.h"  // 包含自定义的Emakefun_MotorDriver.h头文件，提供对Emakefun_MotorDriver类的声明和定义
#include <chrono>                  // 包含chrono头文件，提供时间函数和类
#include <thread>                  // 包含thread头文件，提供线程功能
#include <pigpio.h>                // 包含pigpio库头文件，提供对树莓派GPIO的控制

// 构造函数，初始化Emakefun_MotorDriver对象，设置I2C地址
Emakefun_MotorDriver::Emakefun_MotorDriver(uint8_t addr) { 
  _i2caddr = addr;  // 将传入的I2C地址保存到私有成员变量_i2caddr中
}

// 初始化方法
void Emakefun_MotorDriver::begin(void) {
  Raspi_I2C::init(_i2caddr);  // 调用基类Raspi_I2C的init方法，初始化I2C通信
  reset();                    // 复位PCA9685芯片
}

// 复位PCA9685芯片
void Emakefun_MotorDriver::reset(void) { 
  write8(PCA9685_MODE1, 0x00);  // 向MODE1寄存器写入0x00，复位芯片
}

// 设置PWM频率
void Emakefun_MotorDriver::setPWMFreq(float freq) {
  // 计算预分频值
  float prescaleval = 25000000;  // 固定时钟频率25MHz
  prescaleval /= 4096;           // 12位分辨率
  prescaleval /= freq;           // 计算目标频率对应的分频值
  prescaleval -= 1;              // 减去1得到实际的分频值
  uint8_t prescale = floor(prescaleval + 0.5);  // 四舍五入得到最终的分频值

  uint8_t oldmode = read8(PCA9685_MODE1);       // 读取当前MODE1寄存器的值
  uint8_t newmode = (oldmode & 0x7F) | 0x10;    // 设置睡眠模式，保持低7位不变，设置第4位为1
  write8(PCA9685_MODE1, newmode);               // 进入睡眠模式
  write8(PCA9685_PRESCALE, prescale);           // 设置预分频寄存器
  write8(PCA9685_MODE1, oldmode);               // 恢复MODE1寄存器原来的值

  // 等待5毫秒，确保设置生效
  std::this_thread::sleep_for(std::chrono::milliseconds(5));

  // 启用自动增量模式
  write8(PCA9685_MODE1, oldmode | 0xa1);  // 设置自动增量模式，使传输连续进行
}

// 设置PWM的打开和关闭时间
void Emakefun_MotorDriver::setPWM(uint8_t num, uint16_t on, uint16_t off) {
  // 生成待写入的数据数组
  uint8_t data[] = {
    static_cast<uint8_t>(LED0_ON_L + (num << 2)),  // 计算LED的ON低字节地址
    static_cast<uint8_t>(on & 0xFF),               // ON时间的低8位
    static_cast<uint8_t>(on >> 8),                 // ON时间的高8位
    static_cast<uint8_t>(off & 0xFF),              // OFF时间的低8位
    static_cast<uint8_t>(off >> 8)                 // OFF时间的高8位
  };

  Write(data, sizeof(data));  // 调用基类Raspi_I2C的Write方法，写入数据
}

// 从指定地址读取8位数据
uint8_t Emakefun_MotorDriver::read8(uint8_t addr) { 
  return ReadReg8(addr);  // 调用基类Raspi_I2C的ReadReg8方法，读取数据
}

// 向指定地址写入8位数据
void Emakefun_MotorDriver::write8(uint8_t addr, uint8_t d) { 
  WriteReg8(addr, d);  // 调用基类Raspi_I2C的WriteReg8方法，写入数据
}
