/** 
 * 
 * @author FENN MAI
 * @date 06/05/2024
 * @version Basic 4.0
 * @brief 编码电机初次测试
 */


#ifndef _Emakefun_MotorDriver_H
#define _Emakefun_MotorDriver_H

#include "Raspi_i2c.h"

// PCA9685寄存器地址定义
#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x00
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

/**
 * @class Emakefun_MotorDriver
 * @brief 通过I2C控制PCA9685芯片的类，继承自Raspi_I2C类。
 */
class Emakefun_MotorDriver : public Raspi_I2C {
 public:
  /**
   * @brief 构造函数，初始化I2C地址。
   * @param addr I2C地址，默认值为0x60。
   */
  Emakefun_MotorDriver(uint8_t addr = 0x60);

  /**
   * @brief 初始化I2C连接和PCA9685芯片。
   */
  void begin(void);

  /**
   * @brief 重置PCA9685芯片。
   */
  void reset(void);

  /**
   * @brief 设置PWM频率。
   * @param freq 频率值。
   */
  void setPWMFreq(float freq);

  /**
   * @brief 设置PWM输出。
   * @param num PWM通道号。
   * @param on PWM开启时间。
   * @param off PWM关闭时间。
   */
  void setPWM(uint8_t num, uint16_t on, uint16_t off);

 private:
  uint8_t _i2caddr; ///< I2C地址

  /**
   * @brief 读取8位寄存器的值。
   * @param addr 寄存器地址。
   * @return 寄存器的8位值。
   */
  uint8_t read8(uint8_t addr);

  /**
   * @brief 向8位寄存器写入数据。
   * @param addr 寄存器地址。
   * @param d 写入的数据。
   */
  void write8(uint8_t addr, uint8_t d);
};

#endif // _Emakefun_MotorDriver_H
