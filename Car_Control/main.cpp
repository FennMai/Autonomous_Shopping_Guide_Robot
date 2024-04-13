/***
 * author : Basav
 * last modified date: 12/04/2024
 * 
 * Basic 3.0, date: 10/04/2024
 * testing car control using async and callback
 * 
 */
#include "CarControl.hpp"
#include <iostream>
#include <functional>

using std::cout;
using std::endl;

void onMovementComplete() {
    cout << "Movement complete." << endl;
}

int main() {
    CarControl car;
    car.initialize();
    
    cout << "Moving forward 10 cm." << endl;
    car.moveForward(10.0, onMovementComplete);
    
    cout << "Moving backward 10 cm." << endl;
    car.moveBackward(10.0, onMovementComplete);
    
    cout << "Turning right 90 degrees." << endl;
    car.turnRight(90, onMovementComplete);
    
    cout << "Turning left 90 degrees." << endl;
    car.turnLeft(90, onMovementComplete);
    
    cout << "Stopping car." << endl;
    car.stop();  // This may not require a callback as it's an immediate action.

    cout << "Setting high speed for forward movement and low speed for turning..." << endl;
    car.setSpeed(200, 100); // Set high speed for forward/backward movement and lower speed for turning
    // Repeat some actions with the new speed settings
    car.moveForward(10.0, onMovementComplete); // Move forward 10cm at the new speed
    car.turnLeft(90, onMovementComplete); // Turn left 90 degrees at the new speed
    
    cout << "Test sequence complete." << endl;
    
    cout << "Cleanup resources..." << endl;
    car.cleanup();
    return 0;
}
