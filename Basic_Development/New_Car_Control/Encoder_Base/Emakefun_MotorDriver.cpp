/** 
 * @file Emakefun_MotorDriver.cpp
 * @brief 控制舵机和直流齿轮电机
 * 
 * @author FENN MAI
 * @date 30/05/2024
 * @version Basic 3.0
 * 
 * Version: Basic 3.0
 * 修改日期：2024年5月29日
 * 变更内容：将代码从wiringPi改为pigpio，以控制舵机和直流齿轮电机。
 */

#include "Emakefun_MotorDriver.h"
#include <cmath>
#include <pigpio.h>

/**
 * @brief 构造函数，初始化I2C地址。
 * @param addr I2C地址，默认值为0x60。
 */
Emakefun_MotorDriver::Emakefun_MotorDriver(uint8_t addr) { 
    _i2caddr = addr; 
}

/**
 * @brief 初始化I2C通信并重置PCA9685设备。
 */
void Emakefun_MotorDriver::begin(void) {
    Raspi_I2C::init(_i2caddr); // 初始化I2C通信
    reset(); // 重置PCA9685设备
}

/**
 * @brief 重置PCA9685设备。
 */
void Emakefun_MotorDriver::reset(void) { 
    write8(PCA9685_MODE1, 0x00); 
}

/**
 * @brief 设置PWM频率。
 * @param freq 频率值。
 */
void Emakefun_MotorDriver::setPWMFreq(float freq) {
    float prescaleval = 25000000.0f; // 固定时钟频率
    prescaleval /= 4096.0f; // 分辨率
    prescaleval /= freq; // 频率
    prescaleval -= 1.0f;
    uint8_t prescale = static_cast<uint8_t>(floor(prescaleval + 0.5f));

    uint8_t oldmode = read8(PCA9685_MODE1);
    uint8_t newmode = (oldmode & 0x7F) | 0x10; // 进入睡眠模式
    write8(PCA9685_MODE1, newmode); // 进入睡眠模式
    write8(PCA9685_PRESCALE, prescale); // 设置预分频器
    write8(PCA9685_MODE1, oldmode); // 退出睡眠模式
    gpioDelay(5000); // 等待5ms
    write8(PCA9685_MODE1, oldmode | 0xA1); // 开启自动递增
}

/**
 * @brief 设置PWM输出。
 * @param num PWM通道号。num(0,15)
 * @param on PWM开启时间。PWM 开始时间（0-4095），表示 PWM 周期中信号从低到高的时间点。
 * @param off PWM关闭时间。PWM 结束时间（0-4095），表示 PWM 周期中信号从高到低的时间点。
 */
void Emakefun_MotorDriver::setPWM(uint8_t num, uint16_t on, uint16_t off) {
    uint8_t data[] = {
        // LED0_ON_L 是 PCA9685 寄存器的基地址，用于设置通道 0 的 LED 开始时间低字节。
        // num << 2 将通道号左移两位，这是因为每个通道有四个寄存器（ON_L、ON_H、OFF_L、OFF_H）。
        // LED0_ON_L + (num << 2) 计算得出通道 num 的 ON_L 寄存器地址。
        static_cast<uint8_t>(LED0_ON_L + (num << 2)), 
        // on & 0xFF 获取 on 的低 8 位。
        // on >> 8 获取 on 的高 8 位。
        static_cast<uint8_t>(on & 0xFF),
        static_cast<uint8_t>(on >> 8), 
        // off & 0xFF 获取 off 的低 8 位。
        // off >> 8 获取 off 的高 8 位。
        static_cast<uint8_t>(off & 0xFF), 
        static_cast<uint8_t>(off >> 8)
    };
    Write(data, sizeof(data)); // 写入数据
}

/**
 * @brief 读取8位寄存器的值。
 * @param addr 寄存器地址。
 * @return 寄存器的8位值。
 */
uint8_t Emakefun_MotorDriver::read8(uint8_t addr) { 
    return ReadReg8(addr); 
}

/**
 * @brief 向8位寄存器写入数据。
 * @param addr 寄存器地址。
 * @param d 写入的数据。
 */
void Emakefun_MotorDriver::write8(uint8_t addr, uint8_t d) { 
    WriteReg8(addr, d); 
}
