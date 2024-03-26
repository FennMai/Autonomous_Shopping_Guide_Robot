#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

// 定义DFRobot_DF2301Q的I2C地址
#define I2C_ADDRESS 0x64

int main() {
    int file_i2c;
    int length;
    unsigned char buffer[60] = {0};

    // 打开I2C设备
    char *filename = (char*)"/dev/i2c-1";
    if ((file_i2c = open(filename, O_RDWR)) < 0) {
        std::cout << "Failed to open the i2c bus" << std::endl;
        return 1;
    }

    if (ioctl(file_i2c, I2C_SLAVE, I2C_ADDRESS) < 0) {
        std::cout << "Failed to acquire bus access and/or talk to slave." << std::endl;
        return 1;
    }

    // 以下是如何与设备通信的示例
    // 比如，设置音量为4（这需要替换为相应的I2C命令）
    buffer[0] = 0;    // 命令字节，假设是设置音量的命令
    buffer[1] = 4;    // 音量值
    length = 2;       // 发送数据的长度
    if (write(file_i2c, buffer, length) != length) {
        std::cout << "Failed to write to the i2c bus." << std::endl;
    }

    // 关闭I2C设备
    close(file_i2c);

    return 0;
}
