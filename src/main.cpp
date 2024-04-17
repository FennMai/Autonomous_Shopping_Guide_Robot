#include "ssc_widget.h"
#include"CarControl.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SSC_Widget w;
    w.show();
    return a.exec();

//    try {
//        // Get singleton instance and initialize hardware
//        CarControl* car = CarControl::getInstance();
//        car->initialize();

//        // First, move forward by 100 cm
//        car->moveForward(100, []() {
//            std::cout << "Moved 100 cm forward.\n";
//        });
//        std::this_thread::sleep_for(std::chrono::seconds(3));

//        // Turn right by 90 degrees
//        car->turnRight(90, []() {
//            std::cout << "Turned right by 90 degrees.\n";
//        });
//        std::this_thread::sleep_for(std::chrono::seconds(3));

//        // Move forward by another 50 cm
//        car->moveForward(50, []() {
//            std::cout << "Moved 50 cm forward.\n";
//        });
//        std::this_thread::sleep_for(std::chrono::seconds(3));

//        // Turn left by 90 degrees to face the original direction
//        car->turnLeft(90, []() {
//            std::cout << "Turned left by 90 degrees.\n";
//        });
//        std::this_thread::sleep_for(std::chrono::seconds(3));

//        // Move backward by 50 cm
//        car->moveBackward(50, []() {
//            std::cout << "Moved 50 cm backward.\n";
//        });
//        std::this_thread::sleep_for(std::chrono::seconds(3));

//        car->turnLeft(90, []() {
//            std::cout << "Turned left by 90 degrees.\n";
//        });
//        std::this_thread::sleep_for(std::chrono::seconds(3));

//        // Print the current position and heading of the car
//        std::cout << "Current Position: (" << car->getXPosition() << ", " << car->getYPosition() << ")" << std::endl;
//        std::cout << "Current Heading: " << car->getCurrentAngle() << " degrees" << std::endl;

//        // Finally, stop all motors
//        car->stop();
//        std::cout << "All motors stopped.\n";

//    } catch (const std::exception& e) {
//        std::cerr << "An error occurred: " << e.what() << std::endl;
//        return 1;
//    }
}
