/***
 * author : Basav
 * last modified date: 09/04/2024
 * 
 * Basic 3.0, date: 05/03/2024
 * control the car servo motor+DC using raspberry pi motor driver+L298N motor driver 
 * 
 */
#include "Emakefun_MotorShield.h"
#include <pigpio.h>
#include <iostream>

#define MOTOR_A_PIN_1 5 // GPIO pin for Motor A IN1
#define MOTOR_A_PIN_2 6 // GPIO pin for Motor A IN2
#define MOTOR_B_PIN_1 16 // GPIO pin for Motor B IN1
#define MOTOR_B_PIN_2 20 // GPIO pin for Motor B IN2
//#define MOTOR_PWM_A 19   // PWM pin for Motor A speed control
//#define MOTOR_PWM_B 26   // PWM pin for Motor B speed control

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "Failed to initialize pigpio\n";
        return 1;
    }

    // Servo setup using Emakefun
    Emakefun_MotorShield MS = Emakefun_MotorShield(0x60);
    MS.begin(50);
    Emakefun_Servo *servo = MS.getServo(1);
    servo->writeServo(90); // Center servo

    // L298N motor setup
    gpioSetMode(MOTOR_A_PIN_1, PI_OUTPUT);
    gpioSetMode(MOTOR_A_PIN_2, PI_OUTPUT);
    gpioSetMode(MOTOR_B_PIN_1, PI_OUTPUT);
    gpioSetMode(MOTOR_B_PIN_2, PI_OUTPUT);
    //gpioSetPWMfrequency(MOTOR_PWM_A, 1000); // Set PWM frequency
    //gpioSetPWMfrequency(MOTOR_PWM_B, 1000);

    // Move forward
    gpioWrite(MOTOR_A_PIN_1, PI_HIGH);
    gpioWrite(MOTOR_A_PIN_2, PI_LOW);
    //gpioPWM(MOTOR_PWM_A, 128); // Adjust speed as needed

    gpioWrite(MOTOR_B_PIN_1, PI_HIGH);
    gpioWrite(MOTOR_B_PIN_2, PI_LOW);
    //gpioPWM(MOTOR_PWM_B, 128); // Adjust speed as needed

    gpioDelay(5000000); // 5 seconds

    // Turn left - adjust servo
    servo->writeServo(60); // Adjust angle for left turn
    gpioDelay(2000000); // 2 seconds for turn

    // Move backward
    gpioWrite(MOTOR_A_PIN_1, PI_LOW);
    gpioWrite(MOTOR_A_PIN_2, PI_HIGH);
    gpioWrite(MOTOR_B_PIN_1, PI_LOW);
    gpioWrite(MOTOR_B_PIN_2, PI_HIGH);

    gpioDelay(5000000); // 5 seconds

    // Cleanup
    gpioWrite(MOTOR_A_PIN_1, PI_LOW);
    gpioWrite(MOTOR_A_PIN_2, PI_LOW);
    gpioWrite(MOTOR_B_PIN_1, PI_LOW);
    gpioWrite(MOTOR_B_PIN_2, PI_LOW);
    //gpioPWM(MOTOR_PWM_A, 0);
    // gpioPWM(MOTOR_PWM_B, 0);
    gpioTerminate();
    return 0;
}
