// version: Geo 1.0
#include "DFRobot_DF2301Q_RPi.h"

DFRobot_DF2301Q_RPi::DFRobot_DF2301Q_RPi(const char* i2cDevicePath, int i2cAddr)
    : _i2cAddr(i2cAddr) {
    _file = open(i2cDevicePath, O_RDWR);
    if (_file < 0) {
        std::cerr << "Cannot open I2C device file " << i2cDevicePath << std::endl;
        exit(1);
    }
    if (ioctl(_file, I2C_SLAVE, _i2cAddr) < 0) {
        std::cerr << "Cannot set I2C address to " << i2cAddr << std::endl;
        exit(1);
    }
}

DFRobot_DF2301Q_RPi::~DFRobot_DF2301Q_RPi() {
    if (_file >= 0) {
        close(_file);
    }
}

bool DFRobot_DF2301Q_RPi::begin() {
    // 该设备可能不需要初始化序列，直接返回true
    return true;
}

void DFRobot_DF2301Q_RPi::playByCMDID(uint8_t CMDID) {
    uint8_t data[2] = {DF2301Q_I2C_REG_PLAY_CMDID, CMDID};
    if (write(_file, data, 2) != 2) {
        std::cerr << "Failed to write command ID to the I2C bus." << std::endl;
    }
    // 延迟等待命令执行
    usleep(1000 * 1000);  // 1秒
}

uint8_t DFRobot_DF2301Q_RPi::getCMDID() {
    uint8_t reg = DF2301Q_I2C_REG_CMDID;
    uint8_t value = 0;
    if (write(_file, &reg, 1) != 1 || read(_file, &value, 1) != 1) {
        std::cerr << "Failed to read from the I2C bus." << std::endl;
    }
    return value;
}

int main() {
    DFRobot_DF2301Q_RPi voice("/dev/i2c-1");
    if (!voice.begin()) {
        std::cerr << "Failed to initialize the voice module." << std::endl;
        return 1;
    }
    // 播放命令字ID为0x01的音频
    voice.playByCMDID(0x01);
    while (true) {
        // 获取当前命令字ID
        uint8_t cmdID = voice.getCMDID();
        std::cout << "Current CMD ID: " << std::hex << (int)cmdID << std::endl;

        // 添加适当的延迟
        usleep(1000 * 1000);  // 1秒
    }

    return 0;
}
// 其他方法根据需要实现...

// 请注意，根据你的设备实际支持的命令，你可能需要添加更多的方法来执行不同的操作
