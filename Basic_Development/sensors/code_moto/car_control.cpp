/***
 * author : Basav
 * last modified date: 27/03/2024
 * 
 * Basic 3.0, date: 25/03/2024
 * control the motors with L298N
 * 
 */
#include <iostream>
#include <pigpio.h>

// Define GPIO pin numbers based on BCM numbering
const int IN1 = 17; // BCM GPIO 17
const int IN2 = 27; // BCM GPIO 27 
const int IN3 = 22; // BCM GPIO 22
const int IN4 = 23; // BCM GPIO 23

void setup() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed" << std::endl;
        return;
    }

    // Set GPIO pin modes
    gpioSetMode(IN1, PI_OUTPUT);
    gpioSetMode(IN2, PI_OUTPUT);
    gpioSetMode(IN3, PI_OUTPUT);
    gpioSetMode(IN4, PI_OUTPUT);
}

void moveForward() {
    gpioWrite(IN1, 1);
    gpioWrite(IN2, 0);
    gpioWrite(IN3, 1);
    gpioWrite(IN4, 0);
    std::cout << "Moving Forward" << std::endl;
}

void moveBackward() {
    gpioWrite(IN1, 0);
    gpioWrite(IN2, 1);
    gpioWrite(IN3, 0);
    gpioWrite(IN4, 1);
    std::cout << "Moving Backward" << std::endl;
}

void turnLeft() {
    gpioWrite(IN1, 0);
    gpioWrite(IN2, 1);
    gpioWrite(IN3, 1);
    gpioWrite(IN4, 0);
    std::cout << "Turning Left" << std::endl;
}

void turnRight() {
    gpioWrite(IN1, 1);
    gpioWrite(IN2, 0);
    gpioWrite(IN3, 0);
    gpioWrite(IN4, 1);
    std::cout << "Turning Right" << std::endl;
}

void stop() {
    gpioWrite(IN1, 0);
    gpioWrite(IN2, 0);
    gpioWrite(IN3, 0);
    gpioWrite(IN4, 0);
    std::cout << "Stop" << std::endl;
}

int main() {
    setup();
    moveForward();
    gpioDelay(2000000); // Move forward for 2 seconds
    stop();
    gpioDelay(1000000); // Stop for 1 second

    gpioTerminate(); // Clean up
    return 0;
}
