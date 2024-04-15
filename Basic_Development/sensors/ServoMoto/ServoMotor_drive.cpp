/***
 * author : ZMai
 * last modified date: 04/03/2024
 * drive the led in BCM 18 
*/
#include <iostream>
#include <pigpio.h>

class ServoMotor {
public:
    ServoMotor(int pin) : servoPin(pin) {
        if (gpioInitialise() < 0) {
            std::cerr << "Failed to initialize pigpio library." << std::endl;
            exit(-1);
        }
        gpioSetMode(servoPin, PI_OUTPUT);  // Set GPIO pin to output mode
    }

    ~ServoMotor() {
        gpioTerminate();  // Close pigpio library
    }

    // Control the SG90 servo motor to move to the specified angle
    void move(int angle) {
        int pulseWidth = 500 + (angle * 1000) / 180;  // Calculate pulse width
        gpioServo(servoPin, pulseWidth);  // Set PWM output
        time_sleep(0.5);  // Wait for servo motor to reach the specified position
    }

private:
    const int servoPin;
};

int main() {
    ServoMotor sg90(18);  // Create an SG90 servo motor object, specifying the GPIO pin

    // Example: Move the SG90 servo motor to 90 degrees
    sg90.move(90);

    std::cout << "SG90 servo motor has moved to 90 degrees." << std::endl;

    return 0;
}
