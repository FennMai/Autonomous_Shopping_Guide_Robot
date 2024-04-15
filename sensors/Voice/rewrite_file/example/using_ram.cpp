/*
    // 修正了变量名 cmdID 为 CMDID
    Author: Geo date: 14/04/2024
    Version 1.0 
    using shared memory to communicate between processes
    This example shows how to use shared memory for inter-process communication 
*/
#include <iostream>
#include <chrono>
#include <thread>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <cstring> // For std::memcpy

using namespace boost::interprocess;

// 功能函数，根据命令ID执行特定动作
void performAction(uint8_t CMDID) {
    switch (CMDID) {
        case 23:
            std::cout << "执行动作: 播放音乐" << std::endl;
            break;
        case 121:
            std::cout << "执行动作: 停止播放" << std::endl;
            break;
        default:
            std::cout << "执行未定义动作，命令ID: " << static_cast<int>(CMDID) << std::endl; // 修正了变量名 cmdID 为 CMDID
            break;
    }
}

// 主函数
int main() {
    try {
        // 打开共享内存对象
        shared_memory_object shm(open_only, "CmdIdSharedMemory", read_only);
        mapped_region region(shm, read_only); // 映射共享内存区域

        while (true) {
            uint8_t currentCMDID;
            // 从共享内存读取命令ID
            std::memcpy(&currentCMDID, region.get_address(), sizeof(currentCMDID));

            // 输出当前命令ID并执行相应动作
            std::cout << "收到命令ID: " << static_cast<int>(currentCMDID) << std::endl;
            performAction(currentCMDID); // 根据命令ID执行动作

            // 暂停一定时间再次检查，根据需求调整这里的暂停时间以控制响应频率
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    } catch (const std::exception& ex) {
        std::cerr << "异常发生: " << ex.what() << std::endl;
        return 1; // 返回错误代码
    }

    return 0;
}
