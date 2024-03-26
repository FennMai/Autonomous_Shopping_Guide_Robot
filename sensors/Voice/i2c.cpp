/***
 * author : Geo ZMai
 * last modified date: 26/03/2024
 * 
 * Basic 3.0, date: 26/03/2024
 * drive the DF2301Q
 */
#include "DFRobot_DF2301Q_RPI.h"
#include <iostream>
#include <pigpio.h>
#include <unistd.h> // For sleep()

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed." << std::endl;
        return 1;
    }

    // Create an instance of the DFRobot_DF2301Q_RPi class
    // Assuming the I2C bus is 1 (default on newer Raspberry Pi models) and the device address is 0xXX.
    // Replace 0xXX with your device's actual I2C address.
    DFRobot_DF2301Q_RPi DF2301Q(1, 0x64);

    // Initialize the device
    if (!DF2301Q.begin()) {
        std::cerr << "Communication with device failed, please check connection" << std::endl;
        gpioTerminate(); // Properly terminate pigpio.
        return 1;
    }
    std::cout << "Begin ok!" << std::endl;

    // Set volume, mute mode, and wake time as per the Arduino sketch
    DF2301Q.setVolume(4);
    DF2301Q.setMuteMode(0);
    DF2301Q.setWakeTime(15);

    // Get and print the wake time
    uint8_t wakeTime = DF2301Q.getWakeTime();
    std::cout << "wakeTime = " << static_cast<int>(wakeTime) << std::endl;

    // Play audio by command ID (example: 23)
    DF2301Q.playByCMDID(23);

    while (true) {
        // Get the command ID and react if it's not 0
        uint8_t CMDID = DF2301Q.getCMDID();
        if (CMDID != 0) {
            std::cout << "CMDID = " << static_cast<int>(CMDID) << std::endl;
        }
        sleep(3); // Sleep for 3 seconds before checking again
    }

    gpioTerminate(); // Properly terminate pigpio upon exiting.
    return 0;
}
