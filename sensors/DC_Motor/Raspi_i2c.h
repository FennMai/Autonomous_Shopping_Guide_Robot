#ifndef Raspi_i2c_H
#define Raspi_i2c_H

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <pigpio.h>  // 使用pigpio库

class Raspi_I2C {
 public:
  Raspi_I2C();
  ~Raspi_I2C();  // 析构函数，确保资源被正确释放
  void init(uint8_t address);
  void WriteReg8(uint8_t reg, uint8_t value);
  void WriteReg16(uint8_t reg, uint16_t value);
  void WriteBit8(uint8_t value);
  uint8_t ReadReg8(uint8_t reg);
  uint16_t ReadReg16(uint8_t reg);
  uint8_t ReadBit8();
  uint32_t Write(uint8_t* data, uint32_t size);

 private:
  int handle_;  // 使用handle替代fd_
  uint8_t address_;
};

#endif
