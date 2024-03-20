#include "TCRT5000_Drive.h"
#include <iostream>

// Constructor
TCRT5000::TCRT5000(int doPin) : doPin(doPin) {
    if (gpioInitialise() < 0) {
        std::cerr << "Error initializing pigpio library." << std::endl;
        exit(1);
    }
    gpioSetMode(doPin, PI_INPUT);
}

// Destructor
TCRT5000::~TCRT5000() {
    gpioTerminate();
}

// Method to read and print the sensor's output
void TCRT5000::readAndPrint() {
    while (true) {
        int tcrtOutput = gpioRead(doPin);
        std::cout << (tcrtOutput ? "Detected reflection (black line)." : "No reflection (white surface).") << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
 