#include <iostream>
#include <pigpio.h>
#include <unistd.h> // For the sleep function

class ServoControl
{
public:
    // Constructor initializes GPIO pin and pigpio library
    ServoControl(int gpioPin) : pin(gpioPin)
    {
        // Initialize pigpio library, exit if fails
        if (gpioInitialise() < 0)
        {
            std::cerr << "pigpio initialization failed" << std::endl;
            exit(1);
        }
    }

    // Destructor to clean up resources by terminating pigpio
    ~ServoControl()
    {
        gpioTerminate();
    }

    // Converts an angle in degrees to PWM pulse width
    int angleToPulseWidth(float angle)
    {
        return static_cast<int>((angle + 90) * (2000.0 / 180.0) + 500);
    }

    // Public method to set target angle for the servo
    bool setTargetAngle(float angle)
    {
        if (angle < -90 || angle > 90)
        {
            std::cerr << "Angle must be between -90 and 90" << std::endl;
            return false; // Invalid angle
        }
        moveToAngleSlowly(angle); // Move servo to the validated angle
        return true;              // Valid angle and movement initiated
    }

private:
    int pin;             // GPIO pin number
    float lastAngle = 0; // Record of the last angle, initial value is 0

    // Moves the servo to a specified angle slowly
    void moveToAngleSlowly(float targetAngle)
    {
        float currentAngle = lastAngle;                     // Start from the last angle
        float step = (targetAngle > currentAngle) ? 1 : -1; // Determine the direction of movement

        // Move to the target angle step by step
        while (currentAngle != targetAngle)
        {
            gpioServo(pin, angleToPulseWidth(currentAngle)); // Set servo to current angle
            currentAngle += step;                            // Increment or decrement the angle
            usleep(20000);                                   // Wait for 20ms to slow down the movement
        }

        lastAngle = targetAngle; // Update the last angle to the target angle
    }
};

int main()
{
    ServoControl servo(18); // Instantiate ServoControl, assuming servo connected to GPIO 18

    std::cout << "Enter angle (-90 to 90), enter 'q' to exit:" << std::endl;
    std::string input;
    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, input); // Read angle from standard input
        if (input == "q")
            break; // Exit loop if input is 'q'

        try
        {
            float angle = std::stof(input); // Convert input to floating point number
            servo.setTargetAngle(angle);    // Validate and control servo to move to specified angle
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Please enter a valid angle or 'q' to exit" << std::endl;
        }
    }

    return 0;
}
