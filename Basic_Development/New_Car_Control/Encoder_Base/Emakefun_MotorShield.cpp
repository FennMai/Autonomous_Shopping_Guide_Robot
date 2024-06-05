/** 
 * @file Emakefun_MotorShield.cpp
 * @brief 控制舵机、直流电机和步进电机的实现文件。
 * 
 * @author FENN MAI
 * @date 30/05/2024
 * @version Basic 3.0
 * 
 * 修改日期：2024年5月29日
 * 变更内容：将代码从wiringPi改为pigpio，以控制舵机和直流齿轮电机。
 */

#include "Emakefun_MotorShield.h"
#include <pigpio.h>
#include <algorithm>

// 根据微步数定义微步曲线
#if (MICROSTEPS == 8)
static const uint8_t microstepcurve[] = {0, 50, 98, 142, 180, 212, 236, 250, 255};
#elif (MICROSTEPS == 16)
static const uint8_t microstepcurve[] = {0, 25, 50, 74, 98, 120, 141, 162, 180, 197, 212, 225, 236, 244, 250, 253, 255};
#endif

/**
 * @brief 构造函数，初始化 Emakefun_MotorShield 对象。
 * @param addr I2C 地址，默认为 0x60。
 */
Emakefun_MotorShield::Emakefun_MotorShield(uint8_t addr) {
  _addr = addr;
  _pwm = Emakefun_MotorDriver(_addr);
}

/**
 * @brief 初始化电机驱动板。
 * @param freq PWM 频率，默认为 50 Hz。
 */
void Emakefun_MotorShield::begin(uint16_t freq) {
  // 初始化 pigpio 库。如果在其他地方已经调用过该函数，则可以删除此处的调用。
  if (gpioInitialise() < 0) {
    // 初始化失败
    fprintf(stderr, "Pigpio initialization failed\n");
    return;
  }

  // 初始化 I2C 通信，使用 pigpio 的 I2C 方法。
  _pwm.begin();
  
  _freq = freq;
  // 通过 I2C 通信设置 PWM 频率
  _pwm.setPWMFreq(_freq);

  // 为所有通道设置初始 PWM 值
  for (uint8_t i = 0; i < 16; i++) {
    _pwm.setPWM(i, 0, 0);
  }

  // 考虑在程序结束时调用 gpioTerminate() 以正确停止 pigpio 库。
}

/**
 * @brief 设置指定引脚的 PWM 值。
 * @param pin 引脚编号。
 * @param value PWM 值（0-4095）。
 */
void Emakefun_MotorShield::setPWM(uint8_t pin, uint16_t value) {
  if (value > 4095) {
    _pwm.setPWM(pin, 4096, 0);
  } else {
    _pwm.setPWM(pin, 0, value);
  }
}

/**
 * @brief 设置指定引脚的状态。
 * @param pin 引脚编号。
 * @param value 状态值（0 或 1）。
 */
void Emakefun_MotorShield::setPin(uint8_t pin, uint8_t value) {
  if (value == 0) {
    _pwm.setPWM(pin, 0, 0);
  } else {
    _pwm.setPWM(pin, 4096, 0);
  }
}

/**
 * @brief 获取指定编号的直流电机对象。
 * @param num 电机编号（1-4）。
 * @return 返回直流电机对象的指针，如果编号无效则返回 NULL。
 */
Emakefun_DCMotor *Emakefun_MotorShield::getMotor(uint8_t num) {
  if (num > 4) return NULL;

  num--;

  if (dcmotors[num].motornum == 0) {
    dcmotors[num].motornum = num;
    dcmotors[num].MC = this;
    uint8_t in1 = 0;
    uint8_t in2 = 0;
    if (num == 0) {
      in2 = 13;
      in1 = 11;
    } else if (num == 1) {
      in2 = 8;
      in1 = 10;
    } else if (num == 2) {
      in2 = 4;
      in1 = 2;
    } else if (num == 3) {
      in2 = 7;
      in1 = 5;
    }
    dcmotors[num].IN1pin = in1;
    dcmotors[num].IN2pin = in2;
  }
  return &dcmotors[num];
}

/**
 * @brief 获取指定编号的舵机对象。
 * @param num 舵机编号（1-8）。
 * @return 返回舵机对象的指针，如果编号无效则返回 NULL。
 */
Emakefun_Servo *Emakefun_MotorShield::getServo(uint8_t num) {
  uint8_t pwm_pin[8] = {0, 1, 14, 15, 9, 12, 3, 6};
  if (num > 8) return NULL;
  if (servos[num].servonum == 0) {
    servos[num].servonum = num;
    servos[num].MC = this;
    servos[num].PWMpin = pwm_pin[num - 1];
    servos[num].PWMfreq = _freq;
  }
  return &servos[num];
}

/******************************************
               SERVOS
******************************************/

/**
 * @brief Emakefun_Servo 类的构造函数。
 */
Emakefun_Servo::Emakefun_Servo(void) {
  MC = NULL;
  servonum = 0;
  PWMpin = 0;
  currentAngle = 0;
}

/**
 * @brief 设置舵机的脉冲宽度。
 * @param pulse 脉冲宽度（毫秒）。
 */
void Emakefun_Servo::setServoPulse(double pulse) {
    double pulselength;
    pulselength = 1000000; // 每秒 1,000,000 微秒
    pulselength /= 50;     // 50 Hz
    pulselength /= 4096;   // 12 位分辨率的 PCA9685
    pulse *= 1000;         // 转换为微秒
    pulse /= pulselength;
    MC->setPWM(PWMpin, pulse); // 假设 setPWM 使用 pigpio
}

/**
 * @brief 设置舵机的角度。
 * @param angle 角度值。
 */
void Emakefun_Servo::writeServo(uint8_t angle) {
    double pulse;
    pulse = 0.5 + angle / 90.0; // 控制舵机的基本计算
    setServoPulse(pulse);
    currentAngle = angle;
}

/**
 * @brief 设置舵机的角度和速度。
 * @param angle 角度值。
 * @param speed 速度值（1-10）。
 */
void Emakefun_Servo::writeServo(uint8_t angle, uint8_t speed) {
    double pulse;
    if (speed == 10) {
        pulse = 0.5 + angle / 90.0;
        setServoPulse(pulse);
    } else {
        if (angle < currentAngle) {
            for (int i = currentAngle; i > angle; i--) {
                gpioDelay(4000 * (10 - speed)); // 使用 gpioDelay 进行定时，调整值以适应需求
                pulse = 0.5 + i / 90.0;
                setServoPulse(pulse);
            }
        } else {
            for (int i = currentAngle; i < angle; i++) {
                gpioDelay(4000 * (10 - speed)); // 使用 gpioDelay 进行定时，调整值以适应需求
                pulse = 0.5 + i / 90.0;
                setServoPulse(pulse);
            }
        }
    }
    currentAngle = angle;
}

/**
 * @brief 读取舵机的当前角度。
 * @return 返回当前角度值。
 */
uint8_t Emakefun_Servo::readDegrees() {
    return currentAngle; // 返回最后记录的角度值
}

/******************************************
               MOTORS
******************************************/

/**
 * @brief Emakefun_DCMotor 类的构造函数。
 */
Emakefun_DCMotor::Emakefun_DCMotor(void) {
    MC = NULL;
    motornum = 0;
    _speed = IN1pin = IN2pin = 0;
}

/**
 * @brief 运行直流电机。
 * @param cmd 命令（FORWARD, BACKWARD, RELEASE, BRAKE）。
 */
void Emakefun_DCMotor::run(uint8_t cmd) {
    MDIR = cmd;
    uint8_t pwmValue = std::min(_speed, static_cast<uint8_t>(255)); // 将速度限制在255以内
    switch (cmd) {
        case FORWARD:
            gpioWrite(IN2pin, PI_LOW);  // 设置 IN2pin 低电平
            gpioPWM(IN1pin, pwmValue);  // 在 IN1pin 上输出 PWM 信号
            break;
        case BACKWARD:
            gpioWrite(IN1pin, PI_LOW);  // 设置 IN1pin 低电平
            gpioPWM(IN2pin, pwmValue);  // 在 IN2pin 上输出 PWM 信号
            break;
        case RELEASE:
            gpioWrite(IN1pin, PI_LOW);  // 设置两个引脚低电平
            gpioWrite(IN2pin, PI_LOW);
            break;
        case BRAKE:
            gpioWrite(IN1pin, PI_HIGH); // 设置两个引脚高电平，刹车
            gpioWrite(IN2pin, PI_HIGH);
            break;
    }
}

/**
 * @brief 设置直流电机的速度。
 * @param speed 速度值（0-255）。
 */
void Emakefun_DCMotor::setSpeed(uint8_t speed) {
    _speed = speed; // 假设速度范围为0-255，根据PWM设置可能需要转换
    run(MDIR); // 应用新速度
}
