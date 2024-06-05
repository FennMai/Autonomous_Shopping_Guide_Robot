/** 
 * @file Emakefun_MotorShield.h
 * @brief 控制舵机、直流电机和步进电机的驱动程序。
 * 
 * @author FENN MAI
 * @date 30/05/2024
 * @version Basic 3.0
 * 
 * 修改日期：2024年5月29日
 * 变更内容：将代码从wiringPi改为pigpio，以控制舵机和直流齿轮电机。
 */

#ifndef _Emakefun_MotorShield_h_
#define _Emakefun_MotorShield_h_

#include "Emakefun_MotorDriver.h"

// #define MOTORDEBUG

#define MICROSTEPS 8  ///< 定义微步数（8或16）

// 电机引脚定义
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR4_A 0
#define MOTOR4_B 6
#define MOTOR3_A 5
#define MOTOR3_B 7

// 电机方向定义
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

// 步进电机步数类型定义
#define SINGLE 1
#define DOUBLE 2
#define INTERLEAVE 3
#define MICROSTEP 4

class Emakefun_MotorShield;

/**
 * @class Emakefun_DCMotor
 * @brief 直流电机控制类
 */
class Emakefun_DCMotor {
 public:
  Emakefun_DCMotor(void); ///< 构造函数
  friend class Emakefun_MotorShield; ///< 声明Emakefun_MotorShield为友元类
  void run(uint8_t); ///< 运行电机
  void setSpeed(uint8_t); ///< 设置电机速度

 private:
  uint8_t _speed, IN1pin, IN2pin, MDIR;
  Emakefun_MotorShield *MC;
  uint8_t motornum;
};

/**
 * @class Emakefun_Servo
 * @brief 舵机控制类
 */
class Emakefun_Servo {
 public:
  Emakefun_Servo(void); ///< 构造函数
  friend class Emakefun_MotorShield; ///< 声明Emakefun_MotorShield为友元类
  void setServoPulse(double pulse); ///< 设置舵机脉冲
  void writeServo(uint8_t angle); ///< 设置舵机角度
  void writeServo(uint8_t angle, uint8_t speed); ///< 设置舵机角度和速度
  uint8_t readDegrees(); ///< 读取舵机角度

 private:
  uint8_t PWMpin; ///< 控制伺服电机的PWM引脚编号
  uint16_t PWMfreq; ///< PWM信号的频率
  Emakefun_MotorShield *MC; ///< 指向电机驱动板的指针
  uint8_t servonum; ///< 伺服电机的编号（ID）
  uint8_t currentAngle; ///< 当前伺服电机的角度
};

/**
 * @class Emakefun_MotorShield
 * @brief 电机驱动板控制类
 */
class Emakefun_MotorShield {
 public:
  /**
   * @brief 构造函数
   * @param addr I2C地址，默认值为0x60
   */
  Emakefun_MotorShield(uint8_t addr = 0x60);
  
  void begin(uint16_t freq = 50); ///< 初始化电机驱动板
  void setPWM(uint8_t pin, uint16_t val); ///< 设置PWM输出
  void setPin(uint8_t pin, uint8_t val); ///< 设置引脚输出
  Emakefun_DCMotor *getMotor(uint8_t n); ///< 获取直流电机对象
  //Emakefun_StepperMotor *getStepper(uint16_t steps, uint8_t n); ///< 获取步进电机对象
  Emakefun_Servo *getServo(uint8_t n); ///< 获取舵机对象

 private:
  uint8_t _addr;
  uint16_t _freq;
  Emakefun_Servo servos[8]; ///< 舵机数组
  Emakefun_DCMotor dcmotors[4]; ///< 直流电机数组
  //Emakefun_StepperMotor steppers[2]; ///< 步进电机数组
  Emakefun_MotorDriver _pwm; ///< PWM控制器对象
};

#endif // _Emakefun_MotorShield_h_
