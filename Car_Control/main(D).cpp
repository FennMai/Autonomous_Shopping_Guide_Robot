/***
 * author : Basav
 * last modified date: 11/04/2024
 * 
 * Basic 3.0, date: 10/04/2024
 * testing car control with distance as a parameter instead of time(Delay)
 * 
 */
#include "CarControl.hpp"
#include <iostream>

int main() {
    CarControl car;
    car.initialize(); // Initialize the car control system

    // Test moving forward
    std::cout << "Moving forward 10cm..." << std::endl;
    car.moveForward(10); // Move forward 10cm

    // Test turning left
    std::cout << "Turning left 90 degrees..." << std::endl;
    car.turnLeft(90); // Turn left 90 degrees

    // Test moving backward
    std::cout << "Moving backward 10cm..." << std::endl;
    car.moveBackward(10); // Move backward 10cm

    // Test turning right
    std::cout << "Turning right 90 degrees..." << std::endl;
    car.turnRight(90); // Turn right 90 degrees

    // Test stopping the car
    std::cout << "Stopping the car..." << std::endl;
    car.stop(); // Stop the car immediately

    // Test setting different speeds
    std::cout << "Setting high speed for forward movement and low speed for turning..." << std::endl;
    car.setSpeed(200, 100); // Set high speed for forward/backward movement and lower speed for turning
    // Repeat some actions with the new speed settings
    car.moveForward(10); // Move forward 10cm at the new speed
    car.turnLeft(90); // Turn left 90 degrees at the new speed

    std::cout << "Cleanup resources..." << std::endl;
    car.cleanup(); // Cleanup resources

    std::cout << "Test completed." << std::endl;

    return 0;
}
