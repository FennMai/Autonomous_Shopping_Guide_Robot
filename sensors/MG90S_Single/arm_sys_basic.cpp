#include <iostream> // 包含标准输入输出流库，用于输入输出操作
#include <pigpio.h> // 包含pigpio库，用于控制GPIO
#include <unistd.h> // 包含unistd库，提供sleep函数
#include <vector>   // 包含vector库，用于创建动态数组
#include <thread>   // 包含thread库，用于创建和管理线程

// 定义mg90s类，用于控制MG90S型号的舵机
class mg90s
{
public:
    // 构造函数，接收一个GPIO引脚编号
    mg90s(int gpioPin) : pin(gpioPin)
    {
        if (!initialized)
        { // 检查pigpio库是否已经初始化
            if (gpioInitialise() < 0)
            {                                                             // 初始化pigpio库
                std::cerr << "pigpio initialization failed" << std::endl; // 如果初始化失败，输出错误信息
                exit(1);                                                  // 并退出程序
            }
            initialized = true; // 设置初始化标志为true
        }
        instances++; // 增加实例计数
    }

    // 析构函数
    ~mg90s()
    {
        if (--instances <= 0)
        {                    // 当所有mg90s实例都被销毁时
            gpioTerminate(); // 终止pigpio库
        }
    }

    // 异步设置目标角度的函数
    void setTargetAngleAsync(float angle)
    {
        // 创建一个线程来异步执行setTargetAngle函数
        std::thread([this, angle]()
                    { this->setTargetAngle(angle); })
            .detach(); // 线程分离，允许独立执行
    }

    // 设置目标角度的函数，确保角度值在[-90, 90]范围内
    void setTargetAngle(float angle)
    {
        if (angle < -90 || angle > 90)
        {                                                                 // 检查角度值是否有效
            std::cerr << "Angle must be between -90 and 90" << std::endl; // 如果无效，输出错误信息
            return;                                                       // 并返回
        }
        moveToAngleSlowly(angle); // 如果有效，调用moveToAngleSlowly函数平滑移动到目标角度
    }

private:
    static bool initialized; // 静态变量，标记pigpio库是否已初始化
    static int instances;    // 静态变量，记录mg90s类的实例数量
    int pin;                 // GPIO引脚编号

    // 平滑移动到指定角度的私有函数
    void moveToAngleSlowly(float targetAngle)
    {
        float currentAngle = 0;                                 // 假设起始角度为0
        float step = (targetAngle > currentAngle) ? 0.5 : -0.5; // 根据目标角度确定步进值

        // 循环，逐步改变当前角度，直到达到目标角度
        while (currentAngle != targetAngle)
        {
            currentAngle += step; // 改变当前角度
            if ((step > 0 && currentAngle > targetAngle) || (step < 0 && currentAngle < targetAngle))
            {
                currentAngle = targetAngle; // 防止超过目标角度
            }
            int pulseWidth = angleToPulseWidth(currentAngle); // 计算对应的PWM脉冲宽度
            gpioServo(pin, pulseWidth);                       // 控制舵机转动
            usleep(20000);                                    // 等待20毫秒，以实现平滑移动
        }
    }

    // 将角度转换为PWM脉冲宽度的函数
    int angleToPulseWidth(float angle)
    {
        return static_cast<int>((angle + 90) * (2000.0 / 180.0) + 500); // 根据角度计算PWM脉冲宽度
    }
};

// 初始化静态变量
bool mg90s::initialized = false;
int mg90s::instances = 0;

int main()
{
    // 创建四个mg90s类的实例，分别控制连接到不同GPIO引脚的四个舵机
    mg90s arm_mtr1(17), arm_mtr2(18), arm_mtr3(27), arm_mtr4(22);

    // 异步并平滑地移动arm_mtr1到45度
    // 这里通过一个新线程来执行移动操作，不会阻塞主线程的执行
    arm_mtr1.setTargetAngleAsync(45);

    // 异步并平滑地移动arm_mtr2到20度
    // 同样，这个操作在另一个新线程上执行
    arm_mtr2.setTargetAngleAsync(20);

    // 异步并平滑地移动arm_mtr3到90度
    // 这个操作也是异步执行的，使用了另一个线程
    arm_mtr3.setTargetAngleAsync(90);

    // 异步并平滑地移动arm_mtr4到10度
    // 这个操作在它自己的线程上执行，允许与其他电机同时移动
    arm_mtr4.setTargetAngleAsync(10);

    // 主线程在这里等待，以确保所有电机都有足够的时间完成它们的移动
    // 这个等待时间可能需要根据你的具体应用和电机的实际性能来调整
    sleep(10); // 示例中等待10秒，确保所有电机都到达了目标位置

    return 0;
}
