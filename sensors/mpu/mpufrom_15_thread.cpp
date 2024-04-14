#include <pigpio.h>
#include <thread>

#include "MPU6050.h"
#include "MahonyFilter.h"
#include "UART.h"

using namespace std;

void MPU6050Thread::run() {

    // Initialize interrupt callback function
    initializeMPUISR();

    // Goes into infinite loop
    for(;;) {

        // Do Mahonyfilter algorithm and get three Euler's angles
        if(MPU6050Ins.checkNewData()) {

            // When this method is called, callback to Mahony filter thread will be triggered.
            // Data of IMU will be transmitted to filter.
            MPU6050Ins.getData();

        }

    }

}