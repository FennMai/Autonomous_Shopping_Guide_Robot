/***
 * author : Basav
 * last modified date: 02/04/2024
 * 
 * Basic 3.0, date: 27/03/2024
 * control the DC motor using raspberry pi motor driver 
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
  motorShield.begin(50); // Set the PWM frequency to 50Hz

  Emakefun_DCMotor *myMotor = motorShield.getMotor(1); // Get the first DC motor
  if (myMotor == nullptr) {
    std::cerr << "Failed to initialize motor\n";
    gpioTerminate();
    return -1;
  }

  // Set the motor speed (0-255)
  myMotor->setSpeed(150); // Adjust speed as needed

  while (true) {
    std::cout << "Motor running forward\n";
    myMotor->run(FORWARD);
    gpioDelay(2000 * 1000); // Run for 2 seconds

    std::cout << "Motor stopped\n";
    myMotor->run(RELEASE);
    gpioDelay(1000 * 1000); // Stop for 1 second

    std::cout << "Motor running backward\n";
    myMotor->run(BACKWARD);
    gpioDelay(2000 * 1000); // Run backward for 2 seconds

    std::cout << "Motor stopped\n";
    myMotor->run(RELEASE);
    gpioDelay(1000 * 1000); // Stop for 1 second
  }

  gpioTerminate();
  return 0;
}
