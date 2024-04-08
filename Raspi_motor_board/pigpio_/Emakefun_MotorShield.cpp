/***
 * author : Basav
 * last modified date: 09/04/2024
 * 
 * Basic 3.0, date: 27/03/2024
 * changed the code from wiringPi to pigpio to control the servo and DC gear 
 * 
 */
#include "Emakefun_MotorShield.h"
#include <pigpio.h>
#include <algorithm>

#if (MICROSTEPS == 8)
static const uint8_t microstepcurve[] = {0, 50, 98, 142, 180, 212, 236, 250, 255};
#elif (MICROSTEPS == 16)
static const uint8_t microstepcurve[] = {0, 25, 50, 74, 98, 120, 141, 162, 180, 197, 212, 225, 236, 244, 250, 253, 255};
#endif

Emakefun_MotorShield::Emakefun_MotorShield(uint8_t addr) {
  _addr = addr;
  _pwm = Emakefun_MotorDriver(_addr);
}

void Emakefun_MotorShield::begin(uint16_t freq) {
  // Initialize pigpio library. This should be done once, so if it's already called elsewhere, remove it from here.
  if (gpioInitialise() < 0) {
    // Initialization failed
    fprintf(stderr, "Pigpio initialization failed\n");
    return;
  }

  // Assuming _pwm.begin() initializes I2C communication, it needs to use pigpio's I2C methods.
  _pwm.begin();
  
  _freq = freq;
  // Set PWM frequency using pigpio if needed or through I2C communication handled in _pwm.setPWMFreq()
  _pwm.setPWMFreq(_freq);

  // Set initial PWM values for all channels
  for (uint8_t i = 0; i < 16; i++) {
    _pwm.setPWM(i, 0, 0);
  }

  // Consider adding gpioTerminate() at the end of your program to properly stop the pigpio library.
}

void Emakefun_MotorShield::setPWM(uint8_t pin, uint16_t value) {
  // printf("[%s][%d] pin:%" PRIu8 ", value: %" PRIu16 "\n", __FUNCTION__,
  //        __LINE__, pin, value);
  if (value > 4095) {
    _pwm.setPWM(pin, 4096, 0);
  } else
    _pwm.setPWM(pin, 0, value);
}
void Emakefun_MotorShield::setPin(uint8_t pin, uint8_t value) {
  // printf("[%s][%d] pin:%" PRIu8 ", value: %" PRIu16 "\n", __FUNCTION__,
  //        __LINE__, pin, value);
  if (value == 0)
    _pwm.setPWM(pin, 0, 0);
  else
    _pwm.setPWM(pin, 4096, 0);
}

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
    //    dcmotors[num].PWMpin = pwm;
    dcmotors[num].IN1pin = in1;
    dcmotors[num].IN2pin = in2;
  }
  return &dcmotors[num];
}

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

Emakefun_Servo::Emakefun_Servo(void) {
  MC = NULL;
  servonum = 0;
  PWMpin = 0;
  currentAngle = 0;
}

void Emakefun_Servo::setServoPulse(double pulse) {
    double pulselength;
    pulselength = 1000000; // 1,000,000 us per second
    pulselength /= 50;     // 50 Hz
    pulselength /= 4096;   // 12 bits of resolution for PCA9685
    pulse *= 1000;         // Convert to microseconds
    pulse /= pulselength;
    MC->setPWM(PWMpin, pulse); // Assuming setPWM is adapted to use pigpio
}

void Emakefun_Servo::writeServo(uint8_t angle) {
    double pulse;
    pulse = 0.5 + angle / 90.0; // Basic calculation for servo control
    setServoPulse(pulse);
    currentAngle = angle;
}

void Emakefun_Servo::writeServo(uint8_t angle, uint8_t speed) {
    double pulse;
    if (speed == 10) {
        pulse = 0.5 + angle / 90.0;
        setServoPulse(pulse);
    } else {
        if (angle < currentAngle) {
            for (int i = currentAngle; i > angle; i--) {
                gpioDelay(4000 * (10 - speed)); // Use gpioDelay for timing, adjust value as needed
                pulse = 0.5 + i / 90.0;
                setServoPulse(pulse);
            }
        } else {
            for (int i = currentAngle; i < angle; i++) {
                gpioDelay(4000 * (10 - speed)); // Use gpioDelay for timing, adjust value as needed
                pulse = 0.5 + i / 90.0;
                setServoPulse(pulse);
            }
        }
    }
    currentAngle = angle;
}

uint8_t Emakefun_Servo::readDegrees() {
    return currentAngle; // Returns the last known angle
}

/******************************************
               MOTORS
******************************************/

Emakefun_DCMotor::Emakefun_DCMotor(void) {
    MC = NULL;
    motornum = 0;
    _speed = IN1pin = IN2pin = 0;
}

void Emakefun_DCMotor::run(uint8_t cmd) {
    MDIR = cmd;
    uint8_t pwmValue = std::min(_speed, static_cast<uint8_t>(255)); // Cap the speed at 255
    switch (cmd) {
        case FORWARD:
            gpioWrite(IN2pin, PI_LOW);  // Set IN2pin low
            gpioPWM(IN1pin, pwmValue);  // PWM on IN1pin
            break;
        case BACKWARD:
            gpioWrite(IN1pin, PI_LOW);  // Set IN1pin low
            gpioPWM(IN2pin, pwmValue);  // PWM on IN2pin
            break;
        case RELEASE:
            gpioWrite(IN1pin, PI_LOW);  // Set both pins low
            gpioWrite(IN2pin, PI_LOW);
            break;
        case BRAKE:
            gpioWrite(IN1pin, PI_HIGH); // Set both pins high for braking
            gpioWrite(IN2pin, PI_HIGH);
            break;
    }
}


void Emakefun_DCMotor::setSpeed(uint8_t speed) {
    _speed = speed; // Assume 0-255 for simplicity, conversion may be needed based on your PWM setup.
    run(MDIR); // Apply the new speed
}
