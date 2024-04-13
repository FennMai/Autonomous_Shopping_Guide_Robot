# Control of Driving System

### Developer 
| Author | E-mail |
| ------ | ----- |
| [Linyunn](https://github.com/Linyunn) | 2940201J@student.gla.ac.uk |
| [basav-sketch](https://github.com/basav-sketch) | 2961731P@student.gla.ac.uk |
# 

### Project architecture version (should be edited)
| boost | nlohmann |
| ------ | ------ |
| v1.82.0 | v3.10.2 |

### boost download link: 
https://www.boost.org/doc/libs/1_84_0/more/getting_started/unix-variants.html
### nlohmann download link: 
https://github.com/nlohmann/json.git
### install wiringpi
```
sudo apt-get update
sudo apt-get install wiringpi
```

### install boost
```
cd boost_1_82_0
./bootstrap.sh --prefix=/usr/local
./b2
sudo ./b2 install
// until up not keep going...
cat /usr/include/boost/version.hpp | grep "BOOST_LIB_VERSION"
// if find the error 
sudo apt-get install libboost-all-dev
```

### install nlohmann (should be edited)
```
cd json-develop
mkdir build
cd build/
cmake ..
make
make install
```

#### Transfer documents to Rasberry Pi
```
scp /home/lin/temp/DC_Motor_test.cpp team@192.168.1.116:/home/team
```
:exclamation: (/home/lin/temp/DC_Motor_test.cpp -- the path of the document which should be transferred ！)

:exclamation: (DC_Motor_test.cpp -- document's name ！)

:exclamation: (team@192.168.1.175 — raspberry username & address ！)

:exclamation: (/home/team -- the path where should the document be transmitted to ！)

### Compile Project （CMAKE）
#### Navigate to the project root directory
```
cd Car_Control
```
#### Create a build directory
```
mkdir pp
cd pp
```
#### Run CMake to generate the build system
```
cmake ..
```
#### Build the project
```
make
```
#### Run the executable file
```
./pp
```
or
```
sudo ./pp
```

#### Close the program
```
Ctrl + C
```

### Run the test code: 
```
./main
```
or
```
sudo ./main
```

### if running code in terminal for single file (demo): 
```
1. g++ -std=c++11 server.cpp -o server
g++ -std=c++11 -I../packages/json-develop -o server server.cpp
2. ./server
```
or
```
g++ led.cpp -o led -lpigpio -lrt -lpthread
sudo ./led
```

### websocket Method for connecting to the web end (still needs to be modified)
1. To install cploar in the Raspberry PI, visit https://www.cpolar.com/
2. Run cpolar http 8022 after installation：
```
cpolar http 8022
```
3. Start the web project in the remote system and configure the new WebSocket in /web/src/views/index.vue as the ip address of the cploar
```
this.ws = new WebSocket('ws://xxx.cn');
```
4. Start the Raspberry PI project
```
./run
```
5. Refresh web project
6. The connection is successful. Hardware starts.

## Project Framework and Structure(remark still needs to be modified)
```
Car_Control/
|-- DC_Motor/
|   |-- Emakefun_MotorDrive/
|   |   |-- Emakefun_MotorDrive.cpp          # Manages the rate of balance adjustments.
|   |   |-- Emakefun_MotorDrive.hpp          # Header file for balance rate functions.
|   |-- Emakefun_Motorshield/
|   |   |-- Emakefun_Motorshield.cpp        # Logic for Level of Detail in controller.
|   |   |-- Emakefun_Motorshield.hpp        # Header file for controller Level of Detail.
|   |-- Raspi_i2c/
|   |   |-- Raspi_i2c.cpp       # Implementation of conveyor status checks.
|   |   |-- Raspi_i2c.hpp       # Header file for conveyor status declarations.
|   |-- DC_Motor/
|   |   |-- DC_Motor.cpp                # Manages button interactions for LEDs.
|-- Servo_Motor/
|   |-- Emakefun_MotorDrive/
|   |   |-- Emakefun_MotorDrive.cpp          # Manages the rate of balance adjustments.
|   |   |-- Emakefun_MotorDrive.hpp          # Header file for balance rate functions.
|   |-- Emakefun_Motorshield/
|   |   |-- Emakefun_Motorshield.cpp        # Logic for Level of Detail in controller.
|   |   |-- Emakefun_Motorshield.hpp        # Header file for controller Level of Detail.
|   |-- Raspi_i2c/
|   |   |-- Raspi_i2c.cpp       # Implementation of conveyor status checks.
|   |   |-- Raspi_i2c.hpp       # Header file for conveyor status declarations.
|   |-- Servo_Motor/
|   |   |-- Servo_Motor.cpp                # Manages button interactions for LEDs.
|-- Stepper_Motor/
|   |-- Emakefun_MotorDrive/
|   |   |-- Emakefun_MotorDrive.cpp          # Manages the rate of balance adjustments.
|   |   |-- Emakefun_MotorDrive.hpp          # Header file for balance rate functions.
|   |-- Emakefun_Motorshield/
|   |   |-- Emakefun_Motorshield.cpp        # Logic for Level of Detail in controller.
|   |   |-- Emakefun_Motorshield.hpp        # Header file for controller Level of Detail.
|   |-- Raspi_i2c/
|   |   |-- Raspi_i2c.cpp       # Implementation of conveyor status checks.
|   |   |-- Raspi_i2c.hpp       # Header file for conveyor status declarations.
|   |-- Stepper_Motor/
|   |   |-- Stepper_Motor.cpp                # Manages button interactions for LEDs.
|-- tests/
|   |-- DC_Motor_test.cpp         # Unit tests for the motor driver module.
|   |-- Servo_Motor_test.cpp         # Unit tests for the motor driver module.
|   |-- Stepper_Motor_test.cpp         # Unit tests for the motor driver module.
|-- build/
|   |-- CMakeFiles/
|   |   |-- (Generated by CMake)
|   |-- cmake_install.cmake           # CMake installation script.
|   |-- CMakeCache.txt                # CMake cache file.
|   |-- Makefile                      # Generated Makefile for building the project.
|-- pp.exe                    # CMake configuration file for build settings.
|-- CMakeLists.txt                    # CMake configuration file for build settings.
|-- readme.md                         # The README file for the project with documentation.

```
