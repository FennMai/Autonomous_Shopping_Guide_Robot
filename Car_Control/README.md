# Control of Driving System

### Developer 
| Author | E-mail |
| ------ | ----- |
| [Linyunn](https://github.com/Linyunn) | 2940201J@student.gla.ac.uk |
| [basav-sketch](https://github.com/basav-sketch) | 2961731P@student.gla.ac.uk |
# 

### Overview
This part of the project was developed to create a sophisticated control system for a model car using a Raspberry Pi. The main features include asynchronous motor control, distance-based movement, and smooth servo operation for steering. The part was designed to provide a realistic driving experience by calculating movements in terms of physical distance and angles, enhancing the interaction with the car through precise and responsive controls.

### Project Structure
Directories and Files
```
/CarControl/
```
Contains the primary control classes and scripts for operating the car.
```
/Car_Control/Raspi_motor_board/pigpio_/
```
Contains all basic tests for DC motor and Servo using the extension (Raspberrypi motor driver board) 

### Main Components
```
CarControl.cpp/.hpp
```
These files define and implement the main functionalities of the car control system, including initialization, movement commands, and cleanup procedures.
```
main.cpp
```
The entry point for the application which tests various functionalities of the CarControl class.

### Setup and Installation
#### Prerequisites
Raspberry Pi with the latest version of Raspberry Pi OS installed.
Basic wiring components including jumper wires, motor driver board, and suitable motors for the model car.
GPIO access setup on the Raspberry Pi.

### Software Installation
#### 1. Install pigpio:
Ensure that the Raspberry Pi OS is up to date:
```
sudo apt update
sudo apt full-upgrade
```
Install the pigpio library:
```
sudo apt install pigpio
```
#### 2. Clone the Repository:
Clone the project repository to your local Raspberry Pi:
```
git clone https://github.com/FennMai/Real_Time_Embedded_Team_Project/tree/main/Car_Control
cd CarControl
```
#### 3. Compile the Code:
Compile the project using CMake and Make:
```
mkdir build && cd build
cmake ..
make
```
#### 4. Run the Program:
Execute the compiled binary to test the car control system:
```
./car_control
```

### Usage
To operate the car control system, execute the car_control binary located in the build directory. This program will initialize the motor setup and execute predefined maneuvers to demonstrate the capabilities of the control system. Ensure that your Raspberry Pi is connected to the motor driver and that the motors are properly configured as described in the hardware setup documentation.

### Hardware Setup
1. Motor Connections:
Connect your DC motors to the motor driver board. Ensure that the motor inputs are connected to the correct GPIO pins on the Raspberry Pi as defined in the CarControl.cpp file.

2. Servo Setup:
Attach the servo motor for steering to the designated servo port on the motor driver board.

3. Power Supply:
Ensure that your Raspberry Pi and motor driver have an adequate power supply. It is recommended to use a separate power source for the motors to avoid drawing too much current from the Raspberry Pi.

### Troubleshooting
1. Motor Not Responding:

Check all connections between the Raspberry Pi and the motor driver board.

Ensure that the pigpio daemon is running (sudo pigpiod).

2. Servo Calibration:

If the servo does not accurately align to the specified angles, calibrate the servo limits in the CarControl class.

### Contributing
We welcome contributions from the community. If you would like to improve the car control system, please fork the repository, make your changes, and submit a pull request. You can also open issues for bugs you've found or features you think would be beneficial.

### License
This project is licensed under the MIT License - see the LICENSE.md file for details.

### Acknowledgments
1. Thanks to the Raspberry Pi community for the valuable resources and guides.
2. Special thanks to Emakefun for providing motor driver libraries and examples that were instrumental in the development of this project.
