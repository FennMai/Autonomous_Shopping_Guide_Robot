/***
 * Author : Geo 
 * last modify: 14/04/2024
 * 
 * Version 3.0, date: 26/03/2024
 * drive DF2301Q device
 * multithreading
 * 
 * Version 4.0, date: 14/04/2024
 * rewrite the file.   
 * write CMDID to shared memory
 */
#include "DFbot_voi.h" // 自定义的设备控制头文件
#include <iostream>
#include <pigpio.h>  // 树莓派GPIO控制库
#include <thread>    // C++11线程库
#include <chrono>    // 包含chrono库，用于处理时间
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <cstring>

using namespace std;
using namespace boost::interprocess;

shared_memory_object shm;  // 共享内存对象
mapped_region region;      // 映射区域

// 函数定义：定期检查设备发出的命令ID，并写入共享内存
void checkCMDID(DFRobot_DF2301Q_RPi& device) {
    // 创建共享内存对象，如果不存在则创建之
    shm = shared_memory_object(open_or_create, "CmdIdSharedMemory", read_write);
    shm.truncate(sizeof(uint8_t));  // 设置共享内存的大小
    region = mapped_region(shm, read_write); // 映射共享内存

    while (true) {
        uint8_t CMDID = device.getCMDID(); // 获取当前命令ID
        if (CMDID != 0) {
            cout << "CMDID = " << static_cast<int>(CMDID) << endl; // 如果命令ID不为0，则输出
            memcpy(region.get_address(), &CMDID, sizeof(CMDID));  // 将命令ID写入共享内存
        }
        this_thread::sleep_for(chrono::seconds(3)); // 每3秒检查一次
    }
}

// 主函数
int main() {
    if (gpioInitialise() < 0) { // 初始化GPIO
        cerr << "pigpio fail" << endl;
        return 1;
    }

    // 创建设备控制对象，指定I2C总线1和设备地址0x64
    DFRobot_DF2301Q_RPi DF2301Q(1, 0x64);

    // 尝试开始与设备的通信
    if (!DF2301Q.begin()) {
        cerr << "与设备通信失败，请检查连接。" << endl;
        gpioTerminate(); // 清理GPIO资源
        return 1;
    }
    cout << "设备初始化成功！" << endl;

    DF2301Q.setVolume(4);  // 设置设备音量
    DF2301Q.setMuteMode(0);  // 设置静音模式
    DF2301Q.setWakeTime(15);  // 设置唤醒时间

    uint8_t wakeTime = DF2301Q.getWakeTime();  // 获取并显示设备的唤醒时间
    cout << "当前唤醒时间 = " << static_cast<int>(wakeTime) << endl;

    // 创建一个新线程，用于周期性地检查命令ID return CMID
    thread checkCMDIDThread(checkCMDID, ref(DF2301Q));

    // 主线程中对命令ID进行判断和处理
    uint8_t CMDID = 0;  // 初始命令ID
    if (CMDID == 5) {
        DF2301Q.playByCMDID(2);
        cout << "mapping start" << endl;
    } else if (CMDID == 6) {
        DF2301Q.playByCMDID(2);
        cout << "moving to apple" << endl;
    } else if (CMDID == 7) {
        DF2301Q.playByCMDID(2);
        cout << "moving to check out point" << endl;
    } else if (CMDID == 8) {
        DF2301Q.playByCMDID(2);
        cout << "back to start" << endl;
    } else {
        cout << "没有条件被满足" << endl;
    }

    // 等待子线程结束（实际上子线程是无限循环的）
    checkCMDIDThread.join();

    // 移除共享内存
    shared_memory_object::remove("CmdIdSharedMemory");

    // 程序结束前清理GPIO资源
    gpioTerminate();
    return 0;
}

// cmdid 5 (start mapping)
// cmdid 6 (I want apple)
// cmdid 7 (check out)
// cmdid 8 (moving to start) failed
// cmdid 2 (ok)
// cmdid 10 (moving now) failed
// cmdid 11 (start shopping) waiting for recording