/***
 * author : Basav
 * last modified date: 02/04/2024
 * 
 * Basic 3.0, date: 27/03/2024
 * changed the code from wiringPi to pigpio to control the servo and DC gear 
 * 
 */
#ifndef Raspi_i2c_H
#define Raspi_i2c_H

#include <pigpio.h>
#include <cstdint>
#include <cstdio>

class Raspi_I2C {
public:
    Raspi_I2C();
    ~Raspi_I2C();
    bool init(uint8_t address);
    void WriteReg8(uint8_t reg, uint8_t value);
    void WriteReg16(uint8_t reg, uint16_t value);
    void WriteBit8(uint8_t value);
    uint8_t ReadReg8(uint8_t reg);
    uint16_t ReadReg16(uint8_t reg);
    uint8_t ReadBit8();
    uint32_t Write(uint8_t* data, uint32_t size);

private:
    int fd_;
    uint8_t address_;
};

#endif // Raspi_i2c_H

