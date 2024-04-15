#include "i2c.h"

std::mutex i2c::i2cmtx;

i2c::i2c(int address) {

    {
        std::lock_guard<std::mutex> lock(i2cmtx);
        handle = i2cOpen(i2c_BUS, address, 0); 
    }
    
    if (handle < 0) {
        std::cerr << "Failed to open i2c device" << std::endl;
        exit(1);
    }
}

i2c::~i2c() {
    {
        std::lock_guard<std::mutex> lock(i2cmtx);
        i2cClose(handle); 
        gpioTerminate(); 
    }
}


int i2c::write(char* dataArray, int arrayLength) {

    {
        std::lock_guard<std::mutex> lock(i2cmtx);
        if ((err = i2cWriteDevice(handle, dataArray, arrayLength)) != 0) {
        std::cerr << "Can't write to i2c device " << handle << "." << std::endl;
        //i2cClose(handle);
        //gpioTerminate();
        return err;
        }
    }

    return err;
}

int i2c::read(char* dataArray, int arrayLength, int regAddr) {
    
    {
        std::lock_guard<std::mutex> lock(i2cmtx);
        if ((err = i2cWriteByte(handle, regAddr)) != 0) {
        std::cerr << "Can't write address to i2c device " << handle << " before reading." << std::endl;
        //i2cClose(handle);
        //gpioTerminate();
        return err;
        }

        if (i2cReadDevice(handle, dataArray, arrayLength) <= 0) {
        std::cerr << "Can't read data from i2c device " << handle << "." << std::endl;
        //i2cClose(handle);
        //gpioTerminate();
        return err;
        }
    }

    return err;

}