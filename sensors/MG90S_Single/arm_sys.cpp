#include <iostream>
#include <pigpio.h>
#include <unistd.h> // time sleep
#include <vector>
#include <thread>

// define servo motor class
class mg90s
{
public:
    // initialized
    mg90s(int gpioPin) : pin(gpioPin)
    {
        if (!initialized)
        {
            if (gpioInitialise() < 0)
            {
                std::cerr << "pigpio initialized error" << std::endl;
                exit(1);
            }
            initialized = true;
        }
        instances++;
    }
    ~mg90s()
    {
        if (--instances <= 0)
        {
            gpioTerminate();
        }
    }
    // Async
    void setmoveAsync(float angle)
    {
        std::thread([this, angle]()
                    { this->move2angleAsync_p(angle); })
            .detach();
    }
    // add -90~90 protect
    void move2angleAsync_p(float angle)
    {
        if (angle < -90 || angle > 90)
        {
            std::cerr << "Angle must be between -90 and 90" << std::endl; // 如果无效，输出错误信息
            return;                                                       // 并返回
        }
    }
    move2angle_b(angle);
}

void

task1()
{
    // 任务1的代码
}
int main()
{
    std::thread thread1(task1);
    thread1.join();

    return 0;
}