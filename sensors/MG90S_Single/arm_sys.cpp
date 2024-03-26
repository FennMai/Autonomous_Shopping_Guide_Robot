/***
 * author : ZMai
 * last modified date: 26/03/2024
 * 
 * drive 4 mg90s, let them be controlled like a system(basic 2.0,reference arm_sys.cpp)
 * goal:use the Async method, motor running step by step (basic 2.1)
 * 
 * basic 2.1, date:20/03/2024 :
 * fix the error, when run the setTargetAngleAsync, motor will set 0 angle every time
 * 
 * sys 2.3, date:25/03/2024:
 * drive the system, define the initial joints position
 * 
 */


#include <iostream>   // 用于标准输入输出
#include <pigpio.h>   // 用于GPIO操作
#include <vector>     // 用于存储多个对象
#include <thread>     // 用于多线程编程
#include <functional> // 用于std::function
#include <sstream>    // 用于字符串流操作，包括std::istringstream
#include <string> 
#include <unistd.h> // 包含unistd库，提供sleep函数
#include <iterator>

// 定义mg90s类，代表一个舵机控制器
class mg90s
{
public:
    // 定义回调函数类型，不带参数，返回void
    using CallbackFunction = std::function<void()>;

    // 构造函数，初始化一个舵机控制器
    // pin(gpioPin) 对定义的class 输入gpio口，代表控制该口的输出
    // currentAngle(0.0) 对电机的初始位置设定，假设电机初始角度为0，但针对小臂的电机，可能不是0，可以改
    mg90s(int gpioPin) : pin(gpioPin), currentAngle(0.0)
    {
        // 如果pigpio库还没有初始化，则尝试初始化
        if (!initialized)
        {
            // 如果初始化失败，则打印错误信息并退出程序
            if (gpioInitialise() < 0)
            {
                std::cerr << "pigpio initialization failed." << std::endl;
                exit(1);
            }
            // 标记pigpio库已初始化
            initialized = true;
        }
        // 增加实例计数
        instances++;
    }

    // 析构函数，在对象销毁时调用
    ~mg90s()
    {
        // 如果当前是最后一个实例，则终止pigpio库
        if (--instances <= 0)
        {
            gpioTerminate();
        }
    }

    // 异步设置目标角度的方法，可选地接受一个回调函数
    void setTargetAngleAsync(float angle, CallbackFunction callback = nullptr)
    {
        onTargetReached = callback;                                   // 存储回调函数
        std::thread(&mg90s::detectAngle, this, angle).detach(); // 创建并分离新线程来异步移动舵机
    }

    // input angle pre-processing
    void detectAngle(float angle){
        if(angle < -90){
            angle = -90;
        }
        if (angle>90){
            angle = 90;
        }
        moveToAngleSlowly(angle);
        
    }


private:
    static bool initialized; // 静态成员变量，标记pigpio库是否已初始化
    static int instances;    // 静态成员变量，记录当前类的实例数量
    int pin; // GPIO set 
    float currentAngle;               
    CallbackFunction onTargetReached; // 在达到目标角度后调用的回调函数

    // 私有方法，用于平滑移动舵机到目标角度
    void moveToAngleSlowly(float targetAngle)
    {

        float step = (targetAngle > currentAngle) ? 1 : -1; // 根据目标角度确定步进方向

        // 逐步移动到目标角度
        while (currentAngle != targetAngle)
        {
            currentAngle += step;
            // 防止超调
            if ((step > 0 && currentAngle > targetAngle) || (step < 0 && currentAngle < targetAngle))
            {
                currentAngle = targetAngle;
            }
            int pulseWidth = angleToPulseWidth(currentAngle); // 计算对应的PWM脉冲宽度
            gpioServo(pin, pulseWidth);                       // 设置舵机角度
            usleep(20000);                                    // 20ms延迟以模拟平滑移动
        }

        // 移动完成后，如果设置了回调函数，则调用
        if (onTargetReached)
        {
            onTargetReached();
        }
    }

    // 将角度转换为PWM脉冲宽度的方法
    int angleToPulseWidth(float angle)
    {
        // 根据角度计算PWM脉冲宽度
        return static_cast<int>((angle + 90) * (2000.0 / 180.0) + 500);
    }
};

// 静态成员变量的初始化
bool mg90s::initialized = false;
int mg90s::instances = 0;

// 异步顺序控制多个电机
// 修改moveMotorSequentially函数，使其接受角度值作为参数


/**********main********/
// int main()
// {
//     // 初始化舵机控制对象，每个对象控制一个舵机
//     mg90s arm_mtr1(17), arm_mtr2(18), arm_mtr3(27), arm_mtr4(21);

//     // 异步--移动单个舵机，并在达到目标角度后执行回调
//     // arm_mtr1.setTargetAngleAsync(45, []
//     //                              { std::cout << "arm_mtr1 reached 45 degrees.\n"; });
//     // arm_mtr2.setTargetAngleAsync(20, []
//     //                              { std::cout << "arm_mtr2 reached 20 degrees.\n"; });
//     // arm_mtr3.setTargetAngleAsync(90, []
//     //                              { std::cout << "arm_mtr3 reached 90 degrees.\n"; });
//     // arm_mtr4.setTargetAngleAsync(10, []
//     //                              { std::cout << "arm_mtr4 reached 10 degrees.\n"; });

//     // 使用moveMotorSequentially函数异步顺序控制四个舵机
//     mmoveMotorSequentially(arm_mtr1, angle1, arm_mtr2, angle2, arm_mtr3, angle3, arm_mtr4, angle4);

//     // 此处主线程可以继续执行其他任务，不会被阻塞
//     std::cout << "Motor sequence started, continuing with other tasks...\n";

//     // 为了示例目的，此处使用sleep等待足够时间以确保舵机动作可以完成
//     // 在实际应用中，你应该使用更合适的同步机制来等待或者根据程序的需要决定是否等待
//     std::this_thread::sleep_for(std::chrono::seconds(30));

//     return 0;
// }



// 调试，键盘输入使用
int main() {
    mg90s arm_mtr1(12), arm_mtr2(20), arm_mtr3(16), arm_mtr4(21);

    while (true) {
        std::vector<float> angles(4, 0.0); // 用来存储四个电机的角度
        std::cout << "Enter target angles for 4 motors separated by space (0-180), or type 'exit' to quit:\n";

        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line); // 从标准输入读取一行

        if (line == "exit") { // 检查是否输入了退出命令
            break; // 退出循环
        }
        // 这行代码创建了一个std::istringstream对象iss，并用字符串line初始化。
        // std::istringstream是一个输入字符串流，可以像标准输入流std::cin一样从中读取数据。
        std::istringstream iss(line);
        // 迭代器 每次迭代读取的字符串是以空白字符（空格、换行等）分隔的单词。
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                        std::istream_iterator<std::string>{}};

        if (tokens.size() != 4) {
            std::cerr << "Please enter exactly 4 angles.\n";
            continue;
        }

        try {
            for (int i = 0; i < 4; ++i) {
                angles[i] = std::stof(tokens[i]);
                if (angles[i] < -90 || angles[i] > 90) {
                    throw std::out_of_range("Angle must be between -90 and 90.");
                }
            }

            // 设置电机角度
            arm_mtr1.setTargetAngleAsync(angles[0], []{ std::cout << "Motor 1 reached its target angle.\n"; });
            arm_mtr2.setTargetAngleAsync(angles[1], []{ std::cout << "Motor 2 reached its target angle.\n"; });
            arm_mtr3.setTargetAngleAsync(angles[2], []{ std::cout << "Motor 3 reached its target angle.\n"; });
            arm_mtr4.setTargetAngleAsync(angles[3], []{ std::cout << "Motor 4 reached its target angle.\n"; });
        } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid input. Please enter numbers only.\n";
        } catch (const std::out_of_range &e) {
            std::cerr << e.what() << '\n';
        }
    }

    std::cout << "Exiting...\n";
    // 为了确保所有异步操作完成，这里使用sleep。
    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}