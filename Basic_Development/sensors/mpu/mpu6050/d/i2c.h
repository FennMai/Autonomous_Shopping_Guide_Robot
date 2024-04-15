#ifndef __IIC_H__
#define __IIC_H__

#include <pigpio.h>
#include <iostream>
#include <mutex>

#define i2c_BUS 1

extern std::mutex i2cmtx;

class i2c {

public:

    i2c(int address);

    ~i2c();
    
    int write(char* dataArray, int arrayLength);

    int read(char* dataArray, int arrayLength, int regAddr);

private:

    static std::mutex i2cmtx;
    
    int handle;
    int err;

};



#endif