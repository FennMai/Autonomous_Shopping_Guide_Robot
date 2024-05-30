/******************************************************************
 It will only work with http://www.7gp.cn
 ******************************************************************/

// 防止重复包含头文件
#ifndef _Emakefun_MotorShield_h_
#define _Emakefun_MotorShield_h_

// 包含Emakefun_MotorDriver头文件，提供对MotorDriver类的声明和定义
#include "Emakefun_MotorDriver.h"

// 包含pigpio库头文件，提供对树莓派GPIO的控制
#include <pigpio.h>

// #define MOTORDEBUG // 定义调试标志，用于调试时输出信息

// 定义微步模式，8或16
#define MICROSTEPS 8

// 定义电机控制引脚
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR4_A 0
#define MOTOR4_B 6
#define MOTOR3_A 5
#define MOTOR3_B 7

// 定义电机运行方向和操作
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

// 定义步进电机的步进模式
#define SINGLE 1
#define DOUBLE 2
#define INTERLEAVE 3
#define MICROSTEP 4

// 声明Emakefun_MotorShield类
class Emakefun_MotorShield;

// 直流电机类
class Emakefun_DCMotor {
 public:
  Emakefun_DCMotor(void);  // 构造函数
  friend class Emakefun_MotorShield;  // 声明Emakefun_MotorShield为友元类
  void run(uint8_t);  // 运行电机
  void setSpeed(uint8_t);  // 设置电机速度

 private:
  uint8_t _speed, IN1pin, IN2pin, MDIR;  // 私有成员变量，保存电机状态和控制引脚
  Emakefun_MotorShield *MC;  // 电机盾牌对象指针
  uint8_t motornum;  // 电机编号
};

// 步进电机类
class Emakefun_StepperMotor {
 public:
  Emakefun_StepperMotor(void);  // 构造函数
  void setSpeed(uint16_t);  // 设置步进电机速度
  void step(uint16_t steps, uint8_t dir, uint8_t style = SINGLE);  // 让步进电机移动指定步数
  uint8_t onestep(uint8_t dir, uint8_t style);  // 让步进电机移动一步
  void release(void);  // 释放步进电机

  friend class Emakefun_MotorShield;  // 声明Emakefun_MotorShield为友元类

 private:
  uint32_t usperstep;  // 每步的微秒数
  uint8_t AIN1pin, AIN2pin;  // A相控制引脚
  uint8_t BIN1pin, BIN2pin;  // B相控制引脚
  uint16_t revsteps;  // 每转的步数
  uint8_t currentstep;  // 当前步数
  Emakefun_MotorShield *MC;  // 电机盾牌对象指针
  uint8_t steppernum;  // 步进电机编号
};

// 伺服电机类
class Emakefun_Servo {
 public:
  Emakefun_Servo(void);  // 构造函数
  friend class Emakefun_MotorShield;  // 声明Emakefun_MotorShield为友元类
  void setServoPulse(double pulse);  // 设置伺服脉冲宽度
  void writeServo(uint8_t angle);  // 设置伺服角度
  void writeServo(uint8_t angle, uint8_t speed);  // 设置伺服角度和速度
  uint8_t readDegrees();  // 读取当前伺服角度

 private:
  uint8_t PWMpin;  // PWM控制引脚
  uint16_t PWMfreq;  // PWM频率
  Emakefun_MotorShield *MC;  // 电机盾牌对象指针
  uint8_t servonum, currentAngle;  // 伺服编号和当前角度
};

// 电机盾牌类
class Emakefun_MotorShield {
 public:
  Emakefun_MotorShield(uint8_t addr = 0x60);  // 构造函数，带默认I2C地址
  friend class Emakefun_DCMotor;  // 声明Emakefun_DCMotor为友元类
  void begin(uint16_t freq = 50);  // 初始化方法，带默认PWM频率
  void setPWM(uint8_t pin, uint16_t val);  // 设置PWM值
  void setPin(uint8_t pin, uint8_t val);  // 设置引脚状态
  Emakefun_DCMotor *getMotor(uint8_t n);  // 获取直流电机对象
  Emakefun_StepperMotor *getStepper(uint16_t steps, uint8_t n);  // 获取步进电机对象
  Emakefun_Servo *getServo(uint8_t n);  // 获取伺服电机对象

 private:
  uint8_t _addr;  // I2C地址
  uint16_t _freq;  // PWM频率
  Emakefun_Servo servos[8];  // 伺服电机数组
  Emakefun_DCMotor dcmotors[4];  // 直流电机数组
  Emakefun_StepperMotor steppers[2];  // 步进电机数组
  Emakefun_MotorDriver _pwm;  // MotorDriver对象，用于控制PWM
};

#endif  // 结束条件编译
