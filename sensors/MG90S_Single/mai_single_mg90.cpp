#include <iostream>
#include <pigpio.h>
#include <unistd.h> // 用于sleep函数

// ServoControl类负责控制MG90舵机
class ServoControl {
public:
    // 构造函数，初始化GPIO引脚和PWM频率
    ServoControl(int gpioPin) : pin(gpioPin) {
        // 初始化pigpio库
        if (gpioInitialise() < 0) {
            std::cerr << "pigpio初始化失败" << std::endl;
            exit(1);
        }

        // 设置GPIO为输出模式
        gpioSetMode(pin, PI_OUTPUT);
    }

    // 析构函数，清理资源
    ~ServoControl() {
        gpioTerminate(); // 关闭pigpio库
    }

    // 将角度转换为PWM脉冲宽度
    int angleToPulseWidth(float angle) {
        return static_cast<int>((angle + 90) * (2000.0 / 180.0) + 500);
    }

    // 以较慢的速度移动舵机到指定角度
    void moveToAngleSlowly(float targetAngle) {
        float currentAngle = lastAngle;
        float step = (targetAngle > currentAngle) ? 1 : -1;

        while (currentAngle != targetAngle) {
            gpioServo(pin, angleToPulseWidth(currentAngle));
            currentAngle += step;
            usleep(20000); // 等待20毫秒，减慢移动速度
        }

        lastAngle = targetAngle; // 更新最后的角度值
    }

private:
    int pin; // GPIO引脚编号
    float lastAngle = 0; // 记录上一次的角度，初始为0
};

int main() {
    ServoControl servo(18); // 实例化ServoControl，假设舵机连接到GPIO 18

    std::cout << "Input 脉冲宽度(0.5-2.5)，输入'q'退出：" << std::endl;
    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input); // 从标准输入读取角度
        if (input == "q") break; // 如果输入是'q'，则退出循环

        try {
            float angle = std::stof(input); // 将输入转换为浮点数
            if (angle < -90 || angle > 90) {
                std::cerr << "角度必须在-90到90之间" << std::endl;
            } else {
                servo.moveToAngleSlowly(angle); // 控制舵机移动到指定角度
            }
            
        } catch (const std::invalid_argument& e) {
            std::cerr << "请输入有效的角度或'q'退出" << std::endl;
        }
    }

    return 0;
}
