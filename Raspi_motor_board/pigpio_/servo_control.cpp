#include "Emakefun_MotorShield.h"

int main() {
  Emakefun_MotorShield Pwm = Emakefun_MotorShield();
  Pwm.begin(50); //initialize communication with the motor driver board with a PWM frequency of 50Hz,

  Emakefun_Servo *myServo1 = Pwm.getServo(1);
  //add more if required

  int speed = 9; // The speed parameter

  while (true) {
    myServo1->writeServo(0, speed);/h
    delay(2000);//wait 2 sec
    myServo1->writeServo(90, speed);//rotate angle = 90
    delay(2000);
    myServo1->writeServo(180, speed);
    delay(2000);
    myServo1->writeServo(360, speed);
    delay(2000);
  }
}