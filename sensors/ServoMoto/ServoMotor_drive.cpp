#include <pigpio.h>
#include <unistd.h>

class SG90Servo {
private:
    int pin; // 舵机连接到的GPIO引脚
    int minPulseWidth; // 脉冲宽度的最小值
    int maxPulseWidth; // 脉冲宽度的最大值
    int minAngle; // 角度的最小值
    int maxAngle; // 角度的最大值

public:
    SG90Servo(int gpioPin, int minPW = 500, int maxPW = 2400, int minA = 0, int maxA = 180) : 
        pin(gpioPin), minPulseWidth(minPW), maxPulseWidth(maxPW), minAngle(minA), maxAngle(maxA) {
        gpioInitialise(); // 初始化pigpio库
        gpioSetMode(pin, PI_OUTPUT); // 设置引脚为输出模式
    }

    ~SG90Servo() {
        gpioTerminate(); // 关闭pigpio库
    }

    // 将角度映射到脉冲宽度
    int map(int angle) {
        return (angle - minAngle) * (maxPulseWidth - minPulseWidth) / (maxAngle - minAngle) + minPulseWidth;
    }

    // 控制舵机转到指定角度
    void setAngle(int angle) {
        if (angle < minAngle) {
            angle = minAngle;
        } else if (angle > maxAngle) {
            angle = maxAngle;
        }
        int pulseWidth = map(angle);
        gpioServo(pin, pulseWidth); // 设置舵机的PWM脉冲宽度
        usleep(10000); // 延迟一段时间，舵机转动到目标角度
    }
};

int main() {
    SG90Servo servo(18); // 创建舵机对象，连接到GPIO 18

    servo.setAngle(90); // 将舵机转到90度角度

    return 0;
}
