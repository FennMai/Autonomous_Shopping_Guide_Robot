#include <future>
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
    ServoControl servo1(18); // 实例化ServoControl，假设1号舵机连接到GPIO 18
    ServoControl servo2(23); // 实例化ServoControl，假设2号舵机连接到GPIO 23

    std::cout << "为两个舵机输入不同的角度（-90到90之间），用空格分隔，输入'q'退出：" << std::endl;
    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input); // 从标准输入读取输入
        if (input == "q") break; // 如果输入是'q'，则退出循环

        try {
            std::istringstream iss(input);
            float angle1, angle2;
            iss >> angle1 >> angle2; // 将输入分别解析为两个角度

            if (angle1 < -90 || angle1 > 90 || angle2 < -90 || angle2 > 90) {
                std::cerr << "角度必须在-90到90之间" << std::endl;
            } else {
                // 创建两个异步任务，分别控制两个舵机移动到指定角度
                auto future1 = std::async(std::launch::async, [&servo1, angle1]() {
                    servo1.moveToAngleSlowly(angle1);
                });
                auto future2 = std::async(std::launch::async, [&servo2, angle2]() {
                    servo2.moveToAngleSlowly(angle2);
                });
            }
            
        } catch (const std::invalid_argument& e) {
            std::cerr << "请输入有效的角度或'q'退出" << std::endl;
        }
    }

    return 0;
}
