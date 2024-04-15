/***
 * author : Basav
 * last modified date: 28/03/2024
 * 
 * Basic 3.0, date: 27/03/2024
 * control the motor using raspberry pi motor driver 
 * 
 */
#include "Emakefun_MotorShield.h"
#include "pigpio.h"

int main() {
   if (gpioInitialise() < 0) {
    return 1;
  }
  
  Emakefun_MotorShield Pwm = Emakefun_MotorShield();
  Pwm.begin(50);
  Emakefun_DCMotor *DCmotor1 = Pwm.getMotor(1);
  Emakefun_DCMotor *DCmotor2 = Pwm.getMotor(2);

  DCmotor1->setSpeed(150);
  DCmotor2->setSpeed(300);

  while (1) {
    DCmotor1->run(FORWARD);
    DCmotor2->run(FORWARD);
    gpioDelay(100000);
    
    DCmotor1->run(BACKWARD);
    DCmotor2->run(BACKWARD);
    gpioDelay(100000);
  }
  
  gpioTerminate(); // 清理
  return 0;  
}
