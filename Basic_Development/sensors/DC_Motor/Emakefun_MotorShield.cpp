#include "Emakefun_MotorShield.h"  // 引入Emakefun Motor Shield的头文件

// 根据不同的微步数定义微步曲线数组
#if (MICROSTEPS == 8)
static const uint8_t microstepcurve[] = {0, 50, 98, 142, 180, 212, 236, 250, 255};
#elif (MICROSTEPS == 16)
static const uint8_t microstepcurve[] = {0, 25, 50, 74, 98, 120, 141, 162, 180, 197, 212, 225, 236, 244, 250, 253, 255};
#endif

// Emakefun_MotorShield 类的构造函数，初始化I2C地址和PWM驱动器
Emakefun_MotorShield::Emakefun_MotorShield(uint8_t addr) {
  _addr = addr;  // 保存I2C地址
  _pwm = Emakefun_MotorDriver(_addr);  // 初始化PWM驱动器
}

// 初始化Motor Shield
void Emakefun_MotorShield::begin(uint16_t freq) {
  _pwm.begin();  // 初始化PWM驱动器
  _freq = freq;  // 设置PWM频率
  _pwm.setPWMFreq(_freq);  // 设置PWM频率
  for (uint8_t i = 0; i < 16; i++) {
    _pwm.setPWM(i, 0, 0);  // 关闭所有PWM输出
  }
}

// 设置指定引脚的PWM值
void Emakefun_MotorShield::setPWM(uint8_t pin, uint16_t value) {
  if (value > 4095) {
    _pwm.setPWM(pin, 4096, 0);  // 设置最大值，关闭PWM
  } else {
    _pwm.setPWM(pin, 0, value);  // 设置指定值
  }
}

// 设置指定引脚的状态
void Emakefun_MotorShield::setPin(uint8_t pin, uint8_t value) {
  if (value == 0) {
    _pwm.setPWM(pin, 0, 0);  // 关闭引脚
  } else {
    _pwm.setPWM(pin, 4096, 0);  // 打开引脚
  }
}

// 获取指定编号的直流电机
Emakefun_DCMotor *Emakefun_MotorShield::getMotor(uint8_t num) {
  if (num > 4) return NULL;  // 电机编号超过范围返回空
  num--;  // 调整为数组索引

  if (dcmotors[num].motornum == 0) {
    dcmotors[num].motornum = num;  // 设置电机编号
    dcmotors[num].MC = this;  // 设置控制器
    uint8_t in1 = 0, in2 = 0;
    switch(num) {
      case 0: in1 = 11; in2 = 13; break;
      case 1: in1 = 10; in2 = 8; break;
      case 2: in1 = 2; in2 = 4; break;
      case 3: in1 = 5; in2 = 7; break;
    }
    dcmotors[num].IN1pin = in1;
    dcmotors[num].IN2pin = in2;
  }
  return &dcmotors[num];  // 返回电机实例
}

// 获取指定编号的步进电机
Emakefun_StepperMotor *Emakefun_MotorShield::getStepper(uint16_t steps, uint8_t num) {
  if (num > 2) return NULL;  // 步进电机编号超过范围返回空
  num--;  // 调整为数组索引

  if (steppers[num].steppernum == 0) {
    steppers[num].steppernum = num;  // 设置步进电机编号
    steppers[num].revsteps = steps;  // 设置每转步数
    steppers[num].MC = this;  // 设置控制器
    uint8_t ain1 = 0, ain2 = 0, bin1 = 0, bin2 = 0;
    switch(num) {
      case 0: ain1 = 8; ain2 = 10; bin1 = 11; bin2 = 13; break;
      case 1: ain1 = 4; ain2 = 2; bin1 = 7; bin2 = 5; break;
    }
    steppers[num].AIN1pin = ain1;
    steppers[num].AIN2pin = ain2;
    steppers[num].BIN1pin = bin1;
    steppers[num].BIN2pin = bin2;
  }
  return &steppers[num];  // 返回步进电机实例
}

// 获取指定编号的舵机
Emakefun_Servo *Emakefun_MotorShield::getServo(uint8_t num) {
  uint8_t pwm_pin[8] = {0, 1, 14, 15, 9, 12, 3, 6};
  if (num > 8) return NULL;  // 舵机编号超过范围返回空
  if (servos[num].servonum == 0) {
    servos[num].servonum = num;  // 设置舵机编号
    servos[num].MC = this;  // 设置控制器
    servos[num].PWMpin = pwm_pin[num - 1];  // 设置PWM引脚
    servos[num].PWMfreq = _freq;  // 设置PWM频率
  }
  return &servos[num];  // 返回舵机实例
}

/******************************************
               SERVOS
******************************************/

// Emakefun_Servo 类的构造函数，初始化各参数
Emakefun_Servo::Emakefun_Servo(void) {
  MC = NULL;
  servonum = 0;
  PWMpin = 0;
  currentAngle = 0;
}

// 设置舵机脉冲
void Emakefun_Servo::setServoPulse(double pulse) {
  double pulselength;
  pulselength = 1000000;  // 每秒1,000,000微秒
  pulselength /= 50;  // 50 Hz
  pulselength /= 4096;  // 12位分辨率
  pulse *= 1000;
  pulse /= pulselength;
  MC->setPWM(PWMpin, pulse);
}

// 设置舵机角度
void Emakefun_Servo::writeServo(uint8_t angle) {
  double pulse;
  pulse = 0.5 + angle / 90.0;
  setServoPulse(pulse);
  currentAngle = angle;  // 保存当前角度
}

// 设置舵机角度和速度
void Emakefun_Servo::writeServo(uint8_t angle, uint8_t speed) {
  double pulse;
  if (speed == 10) {
    pulse = 0.5 + angle / 90.0;
    setServoPulse(pulse);
  } else {
    if (angle < currentAngle) {
      for (int i = currentAngle; i > angle; i--) {
        delay(4 * (10 - speed));
        pulse = 0.5 + i / 90.0;
        setServoPulse(pulse);
      }
    } else {
      for (int i = currentAngle; i < angle; i++) {
        delay(4 * (10 - speed));
        pulse = 0.5 + i / 90.0;
        setServoPulse(pulse);
      }
    }
  }
  currentAngle = angle;  // 保存当前角度
}

// 读取当前舵机角度
uint8_t Emakefun_Servo::readDegrees() { return currentAngle; }

/******************************************
               MOTORS
******************************************/

// Emakefun_DCMotor 类的构造函数，初始化各参数
Emakefun_DCMotor::Emakefun_DCMotor(void) {
  MC = NULL;
  motornum = 0;
  _speed = IN1pin = IN2pin = 0;
}

// 运行电机
void Emakefun_DCMotor::run(uint8_t cmd) {
  MDIR = cmd;  // 保存当前指令
  switch (cmd) {
    case FORWARD:
      MC->setPin(IN2pin, 0);  // 先关闭IN2引脚，避免冲突
      MC->setPWM(IN1pin, _speed * 16);  // 设置IN1引脚的PWM
      break;
    case BACKWARD:
      MC->setPin(IN1pin, 0);  // 先关闭IN1引脚，避免冲突
      MC->setPWM(IN2pin, _speed * 16);  // 设置IN2引脚的PWM
      break;
    case RELEASE:
      MC->setPin(IN1pin, 0);  // 关闭IN1引脚
      MC->setPin(IN2pin, 0);  // 关闭IN2引脚
      break;
    case BRAKE:
      MC->setPin(IN1pin, 1);  // 打开IN1引脚
