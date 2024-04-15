#include "CarControl.hpp"
#include <iostream>
#include <functional>

int main() {
    try {
        // Get the singleton instance of the car control system and initialize it
        CarControl* car = CarControl::getInstance();
        car->initialize();

        // Set the desired speed for the movements and turns
        car->setSpeed(30, 45);  // Forward/backward speed, turning speed

        // Command the car to move to a series of coordinates
        std::function<void()> onCompletion = []() {
            std::cout << "Operation completed.\n";
        };

        // First point
        std::cout << "Moving to point (100, 0)\n";
        car->navigateToPoint(100, 0);
        std::cout << "Position: (" << car->getXPosition() << ", " << car->getYPosition() << ")\n";
        std::cout << "Heading: " << car->getCurrentAngle() << " degrees\n";

        // Second point
        std::cout << "Moving to point (100, 100)\n";
        car->navigateToPoint(100, 100);
        std::cout << "Position: (" << car->getXPosition() << ", " << car->getYPosition() << ")\n";
        std::cout << "Heading: " << car->getCurrentAngle() << " degrees\n";

        // Third point
        std::cout << "Moving to point (0, 100)\n";
        car->navigateToPoint(0, 100);
        std::cout << "Position: (" << car->getXPosition() << ", " << car->getYPosition() << ")\n";
        std::cout << "Heading: " << car->getCurrentAngle() << " degrees\n";

        // Finally, stop the car
        car->stop();
        std::cout << "All motors stopped.\n";
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
