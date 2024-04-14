#include "DFbot_voi.h"
#include <pigpio.h>
#include <unistd.h>  

// 定义一个DFRobot_DF2301Q的树莓派版本子类
class DFRobot_DF2301Q_RPi : public DFRobot_DF2301Q {
public:
    // 构造函数，初始化I2C通信
    DFRobot_DF2301Q_RPi(uint8_t i2cBus, uint8_t deviceAddr) : _i2cBus(i2cBus), _deviceAddr(deviceAddr), _i2cHandle(-1) {}

    // 析构函数，关闭I2C通信句柄
    virtual ~DFRobot_DF2301Q_RPi() {
        if (_i2cHandle >= 0) {
            i2cClose(_i2cHandle);  // 关闭I2C连接
        }
    }

    // 初始化函数，设置GPIO和I2C
    bool begin() override {
        if (gpioInitialise() < 0) {
            return false;  // GPIO初始化失败返回false
        }
        _i2cHandle = i2cOpen(_i2cBus, _deviceAddr, 0);  // 打开I2C通道
        return _i2cHandle >= 0;  // 判断I2C句柄是否有效
    }

    // 获取当前命令ID
    uint8_t getCMDID() {
        uint8_t CMDID = 0;
        readReg(DF2301Q_I2C_REG_CMDID, &CMDID);  // 从指定寄存器读取命令ID
        usleep(50000);  // 延时50毫秒，防止读取速率干扰声音模块其他功能
        return CMDID;
    }

    // 根据命令ID播放声音
    void playByCMDID(uint8_t CMDID) {
        writeReg(DF2301Q_I2C_REG_PLAY_CMDID, &CMDID);  // 写入命令ID到寄存器
        sleep(1);  // 延时1秒
    }

    // 获取唤醒时间
    uint8_t getWakeTime() {
        uint8_t wakeTime = 0;
        readReg(DF2301Q_I2C_REG_WAKE_TIME, &wakeTime);  // 从寄存器读取唤醒时间
        return wakeTime;
    }

    // 设置唤醒时间
    void setWakeTime(uint8_t wakeTime) {
        writeReg(DF2301Q_I2C_REG_WAKE_TIME, &wakeTime);  // 将唤醒时间写入寄存器
    }

    // 设置音量
    void setVolume(uint8_t volume) {
        writeReg(DF2301Q_I2C_REG_SET_VOLUME, &volume);  // 将音量值写入寄存器
    }

    // 设置静音模式
    void setMuteMode(uint8_t mode) {
        writeReg(DF2301Q_I2C_REG_SET_MUTE, &mode);  // 将静音模式写入寄存器
    }

protected:
    // 通过I2C写寄存器
    void writeReg(uint8_t reg, const void* pBuf) {
        uint8_t *data = (uint8_t *)pBuf;
        i2cWriteByteData(_i2cHandle, reg, *data);  // 使用pigpio库函数写数据到寄存器
    }

    // 通过I2C读寄存器
    bool readReg(uint8_t reg, void* pBuf) {
        uint8_t *data = (uint8_t*)pBuf;
        int result = i2cReadByteData(_i2cHandle, reg);  // 使用pigpio库函数从寄存器读数据
        if (result >= 0) {
            *data = result;
            return true;  // 读取成功返回true
        }
        return false;  // 读取失败返回false
    }

private:
    int _i2cHandle;       // I2C设备的句柄
    uint8_t _i2cBus;      // 树莓派上的I2C总线编号
    uint8_t _deviceAddr;  // 设备的I2C地址
};
