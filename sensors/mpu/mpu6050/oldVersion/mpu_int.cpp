#include <iostream>
#include <pigpio.h>

// 设定I2C设备的地址和树莓派上的I2C总线（例如，1表示 /dev/i2c-1）
const int I2C_BUS = 1;
const int I2C_ADDR = 0x68; // 更改为你的设备地址

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed." << std::endl;
        return 1;
    }

    // 打开I2C设备
    int i2cHandle = i2cOpen(I2C_BUS, I2C_ADDR, 0);
    if (i2cHandle < 0) {
        std::cerr << "Unable to open I2C device." << std::endl;
        gpioTerminate();
        return 1;
    }

    // 示例：向设备写入数据
    char data[] = {0x00, 0xFF}; // 示例数据，需要根据实际设备来修改
    int writeStatus = i2cWriteDevice(i2cHandle, data, 2);
    if (writeStatus != 0) {
        std::cerr << "Failed to write to the device." << std::endl;
    }

    // 示例：从设备读取数据
    char readData[4]; // 根据需要读取的数据长度来调整
    int readStatus = i2cReadDevice(i2cHandle, readData, 4);
    if (readStatus != 4) { // 确保读取了足够的数据
        std::cerr << "Failed to read from the device." << std::endl;
    } else {
        // 处理读取到的数据
        std::cout << "Data read from device: ";
        for (int i = 0; i < 4; ++i) {
            std::cout << std::hex << (0xFF & readData[i]) << " ";
        }
        std::cout << std::endl;
    }

    // 关闭I2C设备并终止pigpio
    i2cClose(i2cHandle);
    gpioTerminate();
    return 0;
}
