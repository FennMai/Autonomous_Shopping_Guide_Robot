/***
 * author : Basav
 * last modified date: 14/04/2024
 * 
 * Basic 3.0, date: 10/04/2024
 * testing the car control
 * 
 */
#include <iostream>
#include <thread>
#include <chrono>
#include "CarControl.hpp"
#include <functional>

int main() {
    try {
        // Get singleton instance and initialize hardware
        CarControl* car = CarControl::getInstance();
        car->initialize();

        //boundary of the shop
    car->moveForward(160, []() {
        std::cout << "Moved 180 cm forward.\n";
    });

    car->turnRight(90, []() {
        std::cout << "Turned right by 90 degrees.\n";
    });

    car->moveForward(35, []() {
        std::cout << "Moved 100 cm forward.\n";
    });

    car->turnRight(87, []() {
        std::cout << "Turned right by 90 degrees.\n";
    });

    car->moveForward(130, []() {
        std::cout << "Moved 100 cm forward.\n";
    });

    car->turnRight(85, []() {
        std::cout << "Turned right by 90 degrees.\n";
    });

    car->moveForward(45, []() {
        std::cout << "Moved 100 cm forward.\n";
    });

/*        // First, move forward by 100 cm
        car->moveForward(100, []() {
            std::cout << "Moved 100 cm forward.\n";
        });
        std::this_thread::sleep_for(std::chrono::seconds(3));

        // Turn right by 90 degrees
        car->turnRight(90, []() {
            std::cout << "Turned right by 90 degrees.\n";
        });
        std::this_thread::sleep_for(std::chrono::seconds(3));

        // Move forward by another 50 cm
        car->moveForward(50, []() {
            std::cout << "Moved 50 cm forward.\n";
        });
        std::this_thread::sleep_for(std::chrono::seconds(3));

        // Turn left by 90 degrees to face the original direction
        car->turnLeft(90, []() {
            std::cout << "Turned left by 90 degrees.\n";
        });
        std::this_thread::sleep_for(std::chrono::seconds(3));

        // Move backward by 50 cm
        car->moveBackward(50, []() {
            std::cout << "Moved 50 cm backward.\n";
        });
        std::this_thread::sleep_for(std::chrono::seconds(3));

        car->turnLeft(90, []() {
            std::cout << "Turned left by 90 degrees.\n";
        });
        std::this_thread::sleep_for(std::chrono::seconds(3));
*/

        // Print the current position and heading of the car
        std::cout << "Current Position: (" << car->getXPosition() << ", " << car->getYPosition() << ")" << std::endl;
        std::cout << "Current Heading: " << car->getCurrentAngle() << " degrees" << std::endl;

        // Finally, stop all motors
        car->stop();
        std::cout << "All motors stopped.\n";

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
