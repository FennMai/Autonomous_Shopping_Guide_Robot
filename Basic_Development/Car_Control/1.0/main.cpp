/***
 * author : Basav
 * last modified date: 10/04/2024
 * 
 * Basic 3.0, date: 10/03/2024
 * testing the CarControl environment
 * 
 */
#include "CarControl.hpp"
#include <iostream>
#include <unistd.h> // For sleep()

int main() {
    CarControl car;
    car.initialize(); // Initialize the car control

    std::cout << "Testing move forward..." << std::endl;
    car.moveForward(5000); // Move forward for 5 seconds
    sleep(5); // Wait to observe

    std::cout << "Testing turn left..." << std::endl;
    car.turnLeft(2000); // Turn left
    sleep(2); // Wait to observe

    std::cout << "Testing move backward..." << std::endl;
    car.moveBackward(5000); // Move backward for 5 seconds
    sleep(5); // Wait to observe

    std::cout << "Testing turn right..." << std::endl;
    car.turnRight(2000); // Turn right
    sleep(2); // Wait to observe

    std::cout << "Testing stop..." << std::endl;
    car.stop(); // Immediately stop the car
    sleep(2); // Wait to observe

    std::cout << "Testing set speed..." << std::endl;
    car.setSpeed(200); // Set a new speed
    car.moveForward(5000); // Move forward with the new speed
    sleep(5); // Wait to observe

    std::cout << "Cleanup..." << std::endl;
    car.cleanup(); // Cleanup resources

    std::cout << "Test completed." << std::endl;

    return 0;
}
