#include "CarControl.hpp"
#include <iostream>
#include <functional>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main() {
    CarControl* car = CarControl::getInstance();  // Get the singleton instance

    cout << "Initializing car control..." << endl;
    car->initialize();

    cout << "Moving forward 10 cm..." << endl;
    car->moveForward(10.0, []() {
        cout << "Move forward complete!" << endl;
    });

    this_thread::sleep_for(seconds(3)); // Wait to observe

    cout << "Turning right 90 degrees..." << endl;
    car->turnRight(90, []() {
        cout << "Turn right complete!" << endl;
    });

    this_thread::sleep_for(seconds(3)); // Wait to observe

    cout << "Moving backward 10 cm..." << endl;
    car->moveBackward(10.0, []() {
        cout << "Move backward complete!" << endl;
    });

    this_thread::sleep_for(seconds(3)); // Wait to observe

    cout << "Turning left 90 degrees..." << endl;
    car->turnLeft(90, []() {
        cout << "Turn left complete!" << endl;
    });

    this_thread::sleep_for(seconds(3)); // Wait to observe

    cout << "Stopping car..." << endl;
    car->stop();

    cout << "Distance traveled: " << car->getDistanceTraveled() << " cm." << endl;

    return 0;
}
