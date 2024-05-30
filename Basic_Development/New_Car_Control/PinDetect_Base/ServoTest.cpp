/***
 * author : FENN MAI
 * last modified date: 30/05/2024
 * 
 * Basic 3.0, date: 29/05/2024
 * changed the code from wiringPi to pigpio to control the servo and DC gear motor 
 * 
 */

#include "Emakefun_MotorShield.h"
#include <algorithm>
#include <iostream>
#include <pigpio.h>  // 确保包含 pigpio 头文件
using namespace std;

int main() {

  // 初始化 pigpio 库
  if (gpioInitialise() < 0) {
    cerr << "Pigpio initialization failed" << endl;
    return 1;
  }
  
  Emakefun_MotorShield Pwm = Emakefun_MotorShield();
  Pwm.begin(50);
  Emakefun_Servo *myServo1 = Pwm.getServo(1);
  // Emakefun_Servo *myServo2 = Pwm.getServo(2);
  // Emakefun_Servo *myServo3 = Pwm.getServo(3);
  // Emakefun_Servo *myServo4 = Pwm.getServo(4);
  // Emakefun_Servo *myServo5 = Pwm.getServo(5);
  // Emakefun_Servo *myServo6 = Pwm.getServo(6);
  // Emakefun_Servo *myServo7 = Pwm.getServo(7);
  // Emakefun_Servo *myServo8 = Pwm.getServo(8);

  // 速度值是 1 ~ 10 的正整数
  int speed = 6;
  while (true) {
    myServo1->writeServo(0, speed);
    gpioDelay(2000*1000);
    cout<<"turn 0"<<endl;
    myServo1->writeServo(90, speed);
    gpioDelay(2000*1000);
    cout<<"turn 90"<<endl;
    myServo1->writeServo(180, speed);
    gpioDelay(2000*1000);
    cout<<"turn 180"<<endl;
    
  }

  // while (1) {
  //   for (int i = 0; i <= 180; i += 10) {
  //     myServo1->writeServo(i);
  //     myServo2->writeServo(i);
  //     myServo3->writeServo(i);
  //     myServo4->writeServo(i);
  //     myServo5->writeServo(i);
  //     myServo6->writeServo(i);
  //     myServo7->writeServo(i);
  //     myServo8->writeServo(i);
  //     delay(20);
  //   }

  //   for (int i = 180; i >= 0; i -= 10) {
  //     myServo1->writeServo(i);
  //     myServo2->writeServo(i);
  //     myServo3->writeServo(i);
  //     myServo4->writeServo(i);
  //     myServo5->writeServo(i);
  //     myServo6->writeServo(i);
  //     myServo7->writeServo(i);
  //     myServo8->writeServo(i);
  //     delay(20);
  //   }
  // }
    // 终止 pigpio 库
  gpioTerminate();
  return 0;
}
