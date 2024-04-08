# Lidar drive study

## basic drive
```
# via usb, terminal
# step1 
sudo chmod 777 /dev/ttyUSB0
# step2
cd ~/文档/code_program/git_ku/rplidar_sdk/output/Linux/Release
# step3
./ultra_simple --channel --serial /dev/ttyUSB0 460800
```

goal: 
1. change the makefile 2 makelist.txt
path:

### sdk

>include
│   ├── rplidar_cmd.h
│   ├── rplidar_driver.h
│   ├── rplidar.h
│   ├── rplidar_protocol.h
│   ├── rptypes.h
│   ├── sl_crc.h
│   ├── sl_lidar_cmd.h
│   ├── sl_lidar_driver.h 
│   ├── sl_lidar_driver_impl.h
│   ├── sl_lidar.h
│   ├── sl_lidar_protocol.h
│   └── sl_types.h
├── Makefile
└── src
    ├── arch
    │   ├── linux
    │   │   ├── arch_linux.h
    │   │   ├── net_serial.cpp
    │   │   ├── net_serial.h
    │   │   ├── net_socket.cpp
    │   │   ├── thread.hpp
    │   │   ├── timer.cpp
    │   │   └── timer.h
    │   ├── macOS
    │   │   ├── arch_macOS.h
    │   │   ├── net_serial.cpp
    │   │   ├── net_serial.h
    │   │   ├── net_socket.cpp
    │   │   ├── thread.hpp
    │   │   ├── timer.cpp
    │   │   └── timer.h
    │   └── win32
    │       ├── arch_win32.h
    │       ├── net_serial.cpp
    │       ├── net_serial.h
    │       ├── net_socket.cpp
    │       ├── timer.cpp
    │       ├── timer.h
    │       └── winthread.hpp
    ├── dataunpacker
    │   ├── dataunnpacker_commondef.h
    │   ├── dataunnpacker_internal.h
    │   ├── dataunpacker.cpp
    │   ├── dataunpacker.h
    │   ├── dataupacker_namespace.h
    │   └── unpacker
    │       ├── handler_capsules.cpp
    │       ├── handler_capsules.h
    │       ├── handler_hqnode.cpp
    │       ├── handler_hqnode.h
    │       ├── handler_normalnode.cpp
    │       └── handler_normalnode.h
    ├── hal
    │   ├── abs_rxtx.h
    │   ├── assert.h
    │   ├── byteops.h
    │   ├── byteorder.h
    │   ├── event.h
    │   ├── locker.h
    │   ├── socket.h
    │   ├── thread.cpp
    │   ├── thread.h
    │   ├── types.h
    │   ├── util.h
    │   └── waiter.h
    ├── rplidar_driver.cpp
    ├── sdkcommon.h
    ├── sl_async_transceiver.cpp
    ├── sl_async_transceiver.h
    ├── sl_crc.cpp
    ├── sl_lidar_driver.cpp
    ├── sl_lidarprotocol_codec.cpp
    ├── sl_lidarprotocol_codec.h
    ├── sl_serial_channel.cpp
    ├── sl_tcp_channel.cpp
    └── sl_udp_channel.cpp
2 resource:

 https://github.com/trigger1996/Rplidar_Raspberry/tree/master

### structure 

#### drive.cpp


```c++
    /**
    * Lidar scan mode
    */
    struct LidarScanMode
    {
        // Mode id
        sl_u16  id;

        // Time cost for one measurement (in microseconds)
        float   us_per_sample;

        // Max distance in this scan mode (in meters)
        float   max_distance;

        // The answer command code for this scan mode
        sl_u8   ans_type;

        // The name of scan mode (padding with 0 if less than 64 characters)
        char    scan_mode[64];
    };



typedef struct sl_lidar_response_measurement_node_hq_t
{
    sl_u16   angle_z_q14;
    sl_u32   dist_mm_q2;
    sl_u8    quality;
    sl_u8    flag;
} __attribute__((packed)) sl_lidar_response_measurement_node_hq_t;
```