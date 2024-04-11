/**
 * author : ZMai
 * last modified date: 10/04/2024
 * last commit: 
 * driving files: 
 * 
 * basic 1.1, date:10/04/2024:
 * basic from the simple_ultra.cpp, the change is the env --- from makefile to camkelist.txt
 * 
 * 
 **/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "sdk/include/sl_lidar.h" 
#include "sdk/include/sl_lidar_driver.h"
#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

// to change 
#include <unistd.h>
static inline void delay(sl_word_size_t ms){
    while (ms>=1000){
        usleep(1000*1000); // 1s
        ms-=1000;
    };
    if (ms!=0) // ms小于1s的，按照
        usleep(ms*1000);
}


using namespace sl;

void print_usage(int argc, const char * argv[])
{
    printf("Usage:\n"
           " For serial channel\n %s --channel --serial <com port> [baudrate]\n"
           " The baudrate used by different models is as follows:\n"
           "  A1(115200),A2M7(256000),A2M8(115200),A2M12(256000),"
           "A3(256000),S1(256000),S2(1000000),S3(1000000)\n"
		   " For udp channel\n %s --channel --udp <ipaddr> [port NO.]\n"
           " The T1 default ipaddr is 192.168.11.2,and the port NO.is 8089. Please refer to the datasheet for details.\n"
           , argv[0], argv[0]);
    // printf("\n something wrong");
}
// 检测雷达健康 
bool checkSLAMTECLIDARHealth(ILidarDriver * drv) //  * drv 指针
// 通过 drv 访问 ILidarDriver 接口定义的所有公共方法和属性
{
    sl_result     op_result; // uint32_t
    sl_lidar_response_device_health_t healthinfo;

    op_result = drv->getHealth(healthinfo);
    if (SL_IS_OK(op_result)) { // the macro IS_OK is the preperred way to judge whether the operation is succeed.
        printf("SLAMTEC Lidar health status : %d\n", healthinfo.status);
        if (healthinfo.status == SL_LIDAR_STATUS_ERROR) {
            fprintf(stderr, "Error, slamtec lidar internal error detected. Please reboot the device to retry.\n");
            // enable the following code if you want slamtec lidar to be reboot by software
            // drv->reset();
            return false;
        } else {
            return true;
        }

    } else {
        fprintf(stderr, "Error, cannot retrieve the lidar health code: %x\n", op_result);
        return false;
    }
}


bool ctrl_c_pressed; // 全局变量，表示是否按下了Ctrl+C
// SIGINT信号的处理函数，当按下Ctrl+C时会被调用
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

int main(int argc, const char * argv[]) {
	const char * opt_is_channel = NULL; 
	const char * opt_channel = NULL;
    const char * opt_channel_param_first = "/dev/ttyUSB0";
	sl_u32         opt_channel_param_second = 460800;
    sl_u32         baudrateArray[2] = {115200, 256000}; // 支持的波特率列表，C1?
    sl_result     op_result;
	int          opt_channel_type = CHANNEL_TYPE_SERIALPORT;

	// bool useArgcBaudrate = false; // no input，用指定的波特率
    bool useArgcBaudrate = true; // 有输入，用指定的波特率

    IChannel* _channel;

    printf("Ultra simple LIDAR data grabber for SLAMTEC LIDAR.\n"
           "Version: %s\n", SL_LIDAR_SDK_VERSION);

	ILidarDriver * drv = *createLidarDriver();

    if (!drv) {
        fprintf(stderr, "insufficent memory, exit\n");
        exit(-2);
    }

    sl_lidar_response_device_info_t devinfo;
    bool connectSuccess = false; // 连接成功标志
    // 下面的大if，就是在输出connectSuccess，如果成功了，会让connectSuccess = true
    // 默认是用serial -- 0x0
    if(opt_channel_type == CHANNEL_TYPE_SERIALPORT){
        // useArgcBaudrate -- 有波特率输入
        if(useArgcBaudrate){
            // 根据提供的串口名和波特率创建串口通信通道
            _channel = (*createSerialPortChannel(opt_channel_param_first, opt_channel_param_second));
            // 尝试连接到激光雷达设备
            if (SL_IS_OK((drv)->connect(_channel))) {
                // drv 调用getDeviceInfo 函数，获取设备信息
                op_result = drv->getDeviceInfo(devinfo);
                // 如果获取设备信息成功
                if (SL_IS_OK(op_result)) 
                {
                    // 连接成功标志
	                connectSuccess = true;
                }
                else{
                    // 删除实例
                    delete drv;
					drv = NULL;
                }
            }
        }
        // 如果命令行没有输入，计算baudRateArray数组的大小，即支持的波特率数量
        else{ 
            size_t baudRateArraySize = (sizeof(baudrateArray))/ (sizeof(baudrateArray[0]));
             // 遍历所有支持的波特率，进行连接
			for(size_t i = 0; i < baudRateArraySize; ++i)
			{
				_channel = (*createSerialPortChannel(opt_channel_param_first, baudrateArray[i]));
                if (SL_IS_OK((drv)->connect(_channel))) {
                    op_result = drv->getDeviceInfo(devinfo);

                    if (SL_IS_OK(op_result)) 
                    {
	                    connectSuccess = true;
                        break;
                    }
                    else{
                        delete drv;
					    drv = NULL;
                    }
                }
			}
        }
    }

    if (!connectSuccess) {
        (opt_channel_type == CHANNEL_TYPE_SERIALPORT)?
			(fprintf(stderr, "Error, cannot bind to the specified serial port %s.\n"
				, opt_channel_param_first)):(fprintf(stderr, "Error, cannot connect to the specified ip addr %s.\n"
				, opt_channel_param_first));
		
        goto on_finished;
    }

    // print out the device serial number, firmware and hardware version number..
    printf("SLAMTEC LIDAR S/N: ");
    for (int pos = 0; pos < 16 ;++pos) {
        printf("%02X", devinfo.serialnum[pos]);
    }

    printf("\n"
            "Firmware Ver: %d.%02d\n"
            "Hardware Rev: %d\n"
            , devinfo.firmware_version>>8
            , devinfo.firmware_version & 0xFF
            , (int)devinfo.hardware_version);


    // check health 前面定义的函数
    if (!checkSLAMTECLIDARHealth(drv)) {
        // 如果检查结果表明激光雷达的健康状态不佳（函数返回false）
        // 使用goto语句跳转到on_finished标签处
        goto on_finished;
    }

    // 设置中断处理，准备开始扫描!!!
    // 如果 ctrlc -- ctrl_c_pressed = true， 中断！
    signal(SIGINT, ctrlc);

	if(opt_channel_type == CHANNEL_TYPE_SERIALPORT)
        drv->setMotorSpeed(); // 启动激光雷达电机，默认速度--DEFAULT_MOTOR_SPEED--0xFFFFu--65535
    // start scan...
    drv->startScan(0,1);// 开始扫描

    // fetech result and print it out...
    while (1) {
        // 定义一个存储扫描数据的数组
        sl_lidar_response_measurement_node_hq_t nodes[8192];
        // 获取数组可以存储的元素数量-- count
        size_t   count = _countof(nodes);
        // 尝试抓取一轮扫描数据，保存在nodes数组中
        op_result = drv->grabScanDataHq(nodes, count);

        // 如果成功抓取到数据
        if (SL_IS_OK(op_result)) {
            // ascendScanData -- 将扫描数据按照角度进行排序，方便后续处理
            drv->ascendScanData(nodes, count);
            // 遍历并打印每个扫描点的数据
            for (int pos = 0; pos < (int)count ; ++pos) {
                printf("%s theta: %03.2f Dist: %08.2f Q: %d \n", 
                    (nodes[pos].flag & SL_LIDAR_RESP_HQ_FLAG_SYNCBIT) ?"S ":"  ", // 判断是否为一轮扫描的开始,是的话，输出一个s
                    (nodes[pos].angle_z_q14 * 90.f) / 16384.f, // 计算并打印角度值
                    nodes[pos].dist_mm_q2/4.0f, // 计算并打印距离值
                    nodes[pos].quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT); //SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT = 2 ？？？
            }
        }

        if (ctrl_c_pressed){ 
            break;
        }
    }

    drv->stop();// 停止激光雷达的扫描
	delay(200);// 延时200毫秒，确保命令发送完成
    // 如果是串口通信方式，停止激光雷达电机
	if(opt_channel_type == CHANNEL_TYPE_SERIALPORT)
        drv->setMotorSpeed(0);
    // done!
    // 释放激光雷达驱动实例资源并退出程序
on_finished:
    if(drv) {
        delete drv;
        drv = NULL;
    }
    return 0;
}

