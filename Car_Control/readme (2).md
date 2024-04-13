# Hardware Part Of Aircraft Cargo auto Blance System

### Author：Yu Qiao & Yuhan Liu

### Developer 👨‍💻
| Author | E-mail |
| ------ | ----- |
| [🤔️ Yu Qiao(Joey)](http://github.com/qiaoyu113) | 527324363@qq.com |
| [Yuhan Liu](yuhanliu123@outlook.com) | yuhanliu123@outlook.com |
# 

### Project architecture version
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

### install nlohmann
```
cd json-develop
mkdir build
cd build/
cmake ..
make
make install
```

:exclamation: nlohmann下载并编译后要将json-develop中include内的nlohmann移动到/json-develop/nlohmann ！
:exclamation: After nlohmann is downloaded and compiled, move the nlohmann included in json-develop to /json-develop/nlohmann ！

### Compile Project （CMAKE）
#### Navigate to the project root directory
```
cd backend
```
#### Create a build directory
```
mkdir build
cd build
```
#### Run CMake to generate the build system
```
cmake ..
```
#### Build the project
```
cmake --build .
```
Or
```
make
```
#### Run the executable
```
./run
```
or
```
sudo ./run
```

#### Close the tcp
```
sudo lsof -i
sudo kill -9 ID
```

:exclamation: (The packaged build file has been quarantined and will not be uploaded to github ！)
:exclamation: (The packaged Packages include boost and nlohmann file has been quarantined and will not be uploaded to github ！)

### Run the unit test: 
```
./all_tests
```
or
```
sudo ./all_tests
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

### websocket Method for connecting to the web end
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

## Project Framework and Structure
```
Backend/
|-- app/
|   |-- balance_rate/
|   |   |-- balance_rate.cpp          # Manages the rate of balance adjustments.
|   |   |-- balance_rate.hpp          # Header file for balance rate functions.
|   |-- controller/
|   |   |-- controller_lod.cpp        # Logic for Level of Detail in controller.
|   |   |-- controller_lod.hpp        # Header file for controller Level of Detail.
|   |   |-- controller.cpp            # The implementation of the primary controller logic.
|   |   |-- controller.hpp            # The header file defining the controller interface.
|   |-- conveyor_status/
|   |   |-- conveyor_status.cpp       # Implementation of conveyor status checks.
|   |   |-- conveyor_status.hpp       # Header file for conveyor status declarations.
|   |-- led_control/
|   |   |-- button.cpp                # Manages button interactions for LEDs.
|   |   |-- button.hpp                # Header file for button control definitions.
|   |   |-- led.cpp                   # Contains the logic for LED behavior.
|   |   |-- led.hpp                   # Header file for LED operations.
|   |   |-- main.cpp                  # The main entry point for the LED control module.
|   |-- motor_driver/
|   |   |-- motor/
|   |   |   |-- (Contains motor related implementations)
|   |   |-- motor_driver/
|   |   |   |-- (Contains motor driver specific files)
|   |   |-- motor_driver_main.cpp     # Main logic for the motor driver module.
|   |   |-- motor_driver.cpp          # Manages motor driving operations.
|   |   |-- motor_driver.hpp          # Header file for motor driver functions.
|   |-- stepping_motor/
|   |   |-- alarm/
|   |   |   |-- alarm.cpp             # Manages the alarm feature of the stepping motor.
|   |   |   |-- alarm.hpp             # Header file for the alarm feature.
|   |   |-- left/
|   |   |   |-- left.cpp              # Controls the left movement of the stepper motor.
|   |   |   |-- left.hpp              # Header file for left movement definitions.
|   |   |-- pause/
|   |   |   |-- pause.cpp             # Manages the pause functionality of the stepper motor.
|   |   |   |-- pause.hpp             # Header file for pause control definitions.
|   |   |-- right/
|   |   |   |-- right.cpp             # Manages the right movement of the stepper motor.
|   |   |   |-- right.hpp             # Header file for right movement definitions.
|   |-- weight_reader/
|   |   |-- weight_reader.cpp         # Implementation for the weight reader module.
|   |   |-- weight_reader.hpp         # Header file for the weight reader module.
|   |-- weight_sensor/
|       |-- hx711_sensor/
|       |   |-- hx711_sensor.cpp      # Implementation for the HX711 weight sensor logic.
|       |   |-- hx711_sensor.hpp      # Header file for HX711 weight sensor.
|       |-- WeightSensor/
|       |   |-- WeightSensor.cpp      # Manages high-level weight sensor operations.
|       |   |-- WeightSensor.hpp      # Header file for WeightSensor class.
|       |-- main.cpp                  # Main entry point for weight sensor module.
|-- build/
|   |-- CMakeFiles/
|   |   |-- (Generated by CMake)
|   |-- cmake_install.cmake           # CMake installation script.
|   |-- CMakeCache.txt                # CMake cache file.
|   |-- Makefile                      # Generated Makefile for building the project.
|-- include/
|   |-- AbstractScale.h               # Abstract definition for scales.
|   |-- AdvancedHX711.h               # Advanced operations for HX711 sensor.
|   |-- common.h                      # Common definitions and functions.
|   |-- GpioException.h               # Exception handling for GPIO operations.
|   |-- HX711.h                       # Base HX711 sensor class definitions.
|   |-- IntegrityException.h          # Exception for integrity checks.
|   |-- Mass.h                        # Definitions for mass measurements.
|   |-- SimpleHX711.h                 # Simplified operations for HX711 sensor.
|   |-- TimeoutException.h            # Exception handling for timeouts.
|   |-- Utility.h                     # Utility functions.
|   |-- Value.h                       # Value representation definitions.
|   |-- ValueStack.h                  # Stack implementation for value storage.
|   |-- Watcher.h                     # Watcher pattern implementation.
|-- socket/
|   |-- gpio_lock.hpp                 # Header for GPIO locking mechanisms.
|   |-- main.cpp                      # Main logic for socket communication.
|   |-- send_message.cpp              # Logic to send messages via sockets.
|   |-- send_message.hpp              # Header file for message sending functions.
|   |-- server.cpp                    # Server-side socket communication handling.
|   |-- websocket_session/
|   |   |-- websocket_session_button.cpp  # Manages button-related websocket sessions.
|   |   |-- websocket_session_button.hpp  # Header file for button websocket sessions.
|   |   |-- websocket_session_main.cpp   # Main logic for websocket sessions.
|   |   |-- websocket_session_main.hpp   # Header file for main websocket session logic.
|   |   |-- websocket_session.cpp        # Generic websocket session management.
|   |   |-- websocket_session.hpp        # Header file for websocket session definitions.
|-- tests/
|   |-- motor_driver_test.cpp         # Unit tests for the motor driver module.
|   |-- weight_sensor_test.cpp        # Unit tests for weight sensor functionality.
|-- utils/
|   |-- __init__.cpp                  # Initializes the utility module.
|   |-- calculations.cpp              # Includes various calculation functions.
|-- CMakeLists.txt                    # CMake configuration file for build settings.
|-- readme.md                         # The README file for the project with documentation.

```