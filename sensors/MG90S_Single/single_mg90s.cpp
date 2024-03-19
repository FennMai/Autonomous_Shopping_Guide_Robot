/***
 * author : ZMai
 * last modified date: 19/03/2024
 * 
*/

#include <iostream>
#include <pigpio.h>
#include <unistd.h> // 用于usleep函数
#include <cmath>    // 用于fabs函数

// MG90舵机控制类
class MG90Servo {
public:
    // 构造函数，初始化GPIO引脚和PWM相关设置
    MG90Servo(int pin) : servoPin(pin) {
        // 初始化pigpio库
        if (gpioInitialise() < 0) {
            std::cerr << "pigpio初始化失败" << std::endl;
            throw std::runtime_error("pigpio initialization failed");
        }

        // 设置GPIO为PWM输出模式
        gpioSetMode(servoPin, PI_OUTPUT);

        // 设置PWM频率
        gpioSetPWMfrequency(servoPin, PWM_FREQUENCY);

        // 设置PWM范围
        gpioSetPWMrange(servoPin, PWM_RANGE);
    }

    // 析构函数，负责资源的清理
    ~MG90Servo() {
        gpioTerminate(); // 关闭pigpio库
    }

    // 将角度平滑转换到目标角度
    void moveToAngle(float targetAngle, float speed) {
        float currentAngle = lastAngle;
        while (fabs(currentAngle - targetAngle) > 0.5) {
            if (currentAngle < targetAngle) {
                currentAngle += speed;
            } else {
                currentAngle -= speed;
            }

            // 设置舵机角度
            gpioServo(servoPin, angleToPulseWidth(currentAngle));
            usleep(20000); // 等待20毫秒，使转动更平滑
        }
    }

private:
    int servoPin;            // GPIO引脚号
    float lastAngle = 0.0;   // 最后一个角度值，用于平滑转动

    // 将角度转换为PWM脉冲宽度的函数
    int angleToPulseWidth(float angle) {
        return static_cast<int>((angle + 90) * (2000.0 / 180.0) + 500);
    }

    static const int PWM_FREQUENCY = 50; // PWM频率，单位Hz
    static const int PWM_RANGE = 2000;   // PWM范围
};

int main() {
    MG90Servo servo(18); // 创建MG90舵机实例，假设连接到GPIO 18引脚

    // 控制舵机平滑转动到不同角度
    std::cout << "控制MG90舵机平滑转动" << std::endl;
    servo.moveToAngle(-45, 0.5); // 以每次0.5度的速度移动到-45度
    // servo.moveToAngle(45, 0.5);  // 以每次0.5度的速度移动到45度
    // servo.moveToAngle(0, 0.5);   // 以每次0.5度的速度移动到0度

    return 0;
}
