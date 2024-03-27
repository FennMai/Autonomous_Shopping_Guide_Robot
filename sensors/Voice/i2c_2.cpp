/***
 * author : Geo 
 * last modified date: 26/03/2024
 * 
 * Basic 3.0, date: 26/03/2024
 * drive the DF2301Q
 * 
 */
#include "DFRobot_DF2301Q_RPI.h"
#include <iostream>
#include <pigpio.h>
#include <thread>
#include <chrono> // For std::chrono::seconds

void checkCMDID(DFRobot_DF2301Q_RPi& device) {
    while (true) {
        uint8_t CMDID = device.getCMDID();
        if (CMDID != 0) {
            std::cout << "CMDID = " << static_cast<int>(CMDID) << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(3)); // Replace sleep with this
    }
}

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed." << std::endl;
        return 1;
    }

    DFRobot_DF2301Q_RPi DF2301Q(1, 0x64);

    if (!DF2301Q.begin()) {
        std::cerr << "Communication with device failed, please check connection" << std::endl;
        gpioTerminate();
        return 1;
    }
    std::cout << "Begin ok!" << std::endl;

    DF2301Q.setVolume(4);
    DF2301Q.setMuteMode(0);
    DF2301Q.setWakeTime(15);

    uint8_t wakeTime = DF2301Q.getWakeTime();
    std::cout << "wakeTime = " << static_cast<int>(wakeTime) << std::endl;

    DF2301Q.playByCMDID(23);

    // Create a thread to periodically check the CMDID
    std::thread checkCMDIDThread(checkCMDID, std::ref(DF2301Q));

    // Main thread continues to run and can perform other tasks
    // For demonstration, we'll just wait for the CMDID check thread to join
    // (which, in this example, will never happen because the loop runs forever)
    // In a real application, you might have the main thread perform meaningful work or wait for a condition to exit
    checkCMDIDThread.join(); // In real use, you might want to have a way to exit this loop gracefully

    gpioTerminate();
    return 0;
}
