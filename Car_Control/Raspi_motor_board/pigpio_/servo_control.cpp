/***
 * author : Basav
 * last modified date: 02/04/2024
 * 
 * Basic 3.0, date: 27/03/2024
 * control the servo motor using raspberry pi motor driver 
 * 
 */
#include "Emakefun_MotorShield.h"
#include <iostream>
#include <pigpio.h>

int main() {
  if (gpioInitialise() < 0) {
    std::cerr << "Pigpio initialization failed\n";
    return 1; // Exit if pigpio initialization fails
  }

  Emakefun_MotorShield Pwm = Emakefun_MotorShield();
  Pwm.begin(50); // Initialize communication with the motor driver board with a PWM frequency of 50Hz

  Emakefun_Servo *myServo1 = Pwm.getServo(1);
  if (!myServo1) {
    std::cerr << "Failed to get servo\n";
    gpioTerminate();
    return 1; // Exit if getting the servo fails
  }

  int speed = 9; // The speed parameter
   
   /*while (true) {
    myServo1->writeServo(90, speed);//rotate angle = 90
    gpioDelay(2000*1000);
  }*/

  // Example: rotate servo to 90 degrees, then exit
  myServo1->writeServo(90, speed); // Rotate angle = 90
  gpioDelay(2000 * 1000); // 2-second delay

  gpioTerminate(); // Clean up pigpio resources
  return 0;
}
