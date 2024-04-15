/***
 * author : Basav
 * last modified date: 02/04/2024
 * 
 * Basic 3.0, date: 27/03/2024
 * control the car both servo+DC motor using raspberry pi motor driver 
 * 
 */
#include "Emakefun_MotorShield.h"
#include <iostream>
#include <pigpio.h>

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "Pigpio initialization failed\n";
        return -1;
    }

    Emakefun_MotorShield motorShield = Emakefun_MotorShield();
    motorShield.begin(50); // Set PWM frequency to 50Hz

    // Initialize DC motors and servo
    Emakefun_DCMotor *motor1 = motorShield.getMotor(1);
    Emakefun_DCMotor *motor2 = motorShield.getMotor(2);
    Emakefun_Servo *servo1 = motorShield.getServo(1);

    // Ensure motors are available
    if (!motor1 || !motor2 || !servo1) {
        std::cerr << "Initialization failed\n";
        gpioTerminate();
        return -1;
    }

    // Set initial servo position to straight
    servo1->writeServo(90);

    // Move forward
    motor1->setSpeed(255); // Full speed
    motor2->setSpeed(255); // Adjust speed as necessary
    motor1->run(FORWARD);
    motor2->run(FORWARD);
    gpioDelay(5000 * 1000); // 5 seconds

    // Turn right
    servo1->writeServo(60); // Adjust for right turn
    gpioDelay(2000 * 1000); // Adjust time for completing the turn

    // Move forward again
    servo1->writeServo(90); // Straighten wheels
    gpioDelay(5000 * 1000); // Move forward for 5 seconds

    // Turn left
    servo1->writeServo(120); // Adjust for left turn
    gpioDelay(2000 * 1000); // Adjust time for completing the turn

    // Move backward to original position
    motor1->run(BACKWARD);
    motor2->run(BACKWARD);
    gpioDelay(5000 * 1000); // Move backward for 5 seconds

    // Stop motors and servo
    motor1->run(RELEASE);
    motor2->run(RELEASE);
    servo1->writeServo(90); // Return to center

    gpioTerminate();
    return 0;
}
