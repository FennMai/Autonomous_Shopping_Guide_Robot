/** 
 * 
 * @author FENN MAI
 * @date 06/05/2024
 * @version Basic 4.0
 * @brief 编码电机初次测试
 */


#ifndef Raspi_i2c_H // 头文件保护，防止重复包含
#define Raspi_i2c_H

#include <pigpio.h> // 包含pigpio库，用于Raspberry Pi的GPIO控制
#include <cstdint> // 包含标准整数类型
#include <cstdio> // 包含标准输入/输出函数

class Raspi_I2C {
public:
    Raspi_I2C(); // 构造函数
    ~Raspi_I2C(); // 析构函数
    bool init(uint8_t address); // 使用指定地址初始化I2C
    void WriteReg8(uint8_t reg, uint8_t value); // 向8位寄存器写入8位值
    void WriteReg16(uint8_t reg, uint16_t value); // 向8位寄存器写入16位值
    void WriteBit8(uint8_t value); // 写入一个8位值
    uint8_t ReadReg8(uint8_t reg); // 从8位寄存器读取8位值
    uint16_t ReadReg16(uint8_t reg); // 从8位寄存器读取16位值
    uint8_t ReadBit8(); // 读取一个8位值
    uint32_t Write(uint8_t* data, uint32_t size); // 写入一块数据

private:
    int fd_; // I2C设备的文件描述符
    uint8_t address_; // 设备的I2C地址
};

#endif // Raspi_i2c_H
