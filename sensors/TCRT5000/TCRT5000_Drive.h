#ifndef TCRT5000_H
#define TCRT5000_H

#include <pigpio.h>
#include <chrono>
#include <thread>

class TCRT5000 {
public:
    // Constructor
    TCRT5000(int doPin);

    // Destructor
    ~TCRT5000();

    // Method to read and print the sensor's output
    void readAndPrint();

private:
    int doPin;
};

#endif // TCRT5000_H
