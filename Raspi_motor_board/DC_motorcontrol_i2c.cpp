#include <iostream>
#include <pigpio.h>

// Adjust these GPIO pin numbers to match how you've connected your motor driver to the Raspberry Pi
const int motorPlusPin = 17; // GPIO pin connected to motor+
const int motorMinusPin = 27; // GPIO pin connected to motor-

void initializeMotor() {
    if (gpioInitialise() < 0) {
        std::cerr << "Pigpio initialization failed" << std::endl;
        exit(1);
    }

    // Set GPIO pin modes
    gpioSetMode(motorPlusPin, PI_OUTPUT);
    gpioSetMode(motorMinusPin, PI_OUTPUT);

    // Ensure motor is stopped initially
    gpioWrite(motorPlusPin, 0);
    gpioWrite(motorMinusPin, 0);
}

void runMotor(bool direction, int speed) {
    // Assuming the motor driver controls direction by making one pin high and the other low
    // Adjust this logic if your motor driver works differently
    if (direction) {
        gpioWrite(motorPlusPin, 1);
        gpioWrite(motorMinusPin, 0);
    } else {
        gpioWrite(motorPlusPin, 0);
        gpioWrite(motorMinusPin, 1);
    }

    // Adjust speed by PWM on one of the direction pins (choose one that works for your setup)
    gpioPWM(motorPlusPin, speed);
}

void stopMotor() {
    gpioWrite(motorPlusPin, 0);
    gpioWrite(motorMinusPin, 0);
}

int main() {
    initializeMotor();

    // Example: run motor forward at medium speed for 2 seconds, then stop and reverse
    runMotor(true, 128); // Run forward at medium speed
    gpioDelay(2000 * 1000); // Delay for 2 seconds
    stopMotor();
    gpioDelay(1000 * 1000); // Delay for 1 second before reversing
    runMotor(false, 128); // Run backward at medium speed
    gpioDelay(2000 * 1000); // Delay for 2 seconds
    stopMotor();

    gpioTerminate();
    return 0;
}
