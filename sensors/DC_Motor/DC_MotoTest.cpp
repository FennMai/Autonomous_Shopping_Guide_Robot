#include "Emakefun_MotorShield.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <pigpio.h>

// 用于替代 delay 的辅助函数
void delay(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int main() {
    
    if (gpioInitialise() < 0){
        std::cerr << "abc" << std::endl;
        return 1;
    }
  
  Emakefun_MotorShield Pwm;
  Emakefun_StepperMotor *StepperMotor_1 = Pwm.getStepper(200, 1);
  Emakefun_StepperMotor *StepperMotor_2 = Pwm.getStepper(200, 2);
  Pwm.begin(1600);
  StepperMotor_1->setSpeed(400);
  StepperMotor_2->setSpeed(400);

  while (1) {
    StepperMotor_1->step(200, FORWARD, DOUBLE);  // 电机1正转1圈 200步
    StepperMotor_1->release();
    StepperMotor_2->step(200, FORWARD, SINGLE);  // 电机2正转1圈 200步
    StepperMotor_2->release();
    delay(1000);
    StepperMotor_1->step(200, BACKWARD, DOUBLE);  // 电机1反转1圈 200步
    StepperMotor_1->release();
    StepperMotor_2->step(200, BACKWARD, SINGLE);  // 电机2反转1圈 200步
    StepperMotor_2->release();
    delay(1000);
  }
  gpioTerminate();
  return 0;
}

