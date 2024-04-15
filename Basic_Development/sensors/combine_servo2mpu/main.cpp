#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <math.h>
#include <iostream>

#define MPU6050_ADDR 0x68
#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B

class ServoMotor {
public:
    ServoMotor(int pin) : servoPin(pin) {
        if (gpioInitialise() < 0) {
            std::cerr << "Failed to initialize pigpio library." << std::endl;
            exit(-1);
        }
        gpioSetMode(servoPin, PI_OUTPUT);  // Set GPIO pin to output mode
    }

    ~ServoMotor() {
        gpioTerminate();  // Close pigpio library
    }

    void move(int angle) {
        int pulseWidth = 500 + (angle * 1000) / 180;  // Calculate pulse width
        gpioServo(servoPin, pulseWidth);  // Set PWM output
        time_sleep(0.5);  // Wait for the servo motor to reach the specified position
    }

private:
    const int servoPin;
};

int readAccelData(int handle, uint8_t reg) {
    int high = i2cReadByteData(handle, reg);
    int low = i2cReadByteData(handle, reg + 1);
    int value = (high << 8) + low;
    if (value >= 0x8000) value -= 0x10000; // Convert to signed
    return value;
}

// Maps ax values to servo angles (0° to 180°)
// Adjusted for ax values ranging from -1 to 1
int mapAxToServoAngle(double ax) {
    // Map ax from -1 to 1 to 0° to 180°
    double angle = (ax + 1) / 2 * 180; // Linear mapping
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    return static_cast<int>(angle);
}

int main() {
    ServoMotor sg90(18);  // Initialize the servo motor on GPIO pin 18
    
    int handle = i2cOpen(1, MPU6050_ADDR, 0);
    if (handle < 0) {
        fprintf(stderr, "Failed to open I2C connection\n");
        return 1;
    }

    i2cWriteByteData(handle, PWR_MGMT_1, 0); // Wake up the MPU6050

    while (1) {
        int accelX = readAccelData(handle, ACCEL_XOUT_H);
        int accelY = readAccelData(handle, ACCEL_XOUT_H + 2); // Address of the Y-axis high byte
        int accelZ = readAccelData(handle, ACCEL_XOUT_H + 4); // Address of the Z-axis high byte
        
        // Convert accelerometer data to g (9.8 m/s^2)
        double ax = accelX / 16384.0;
        double ay = accelY / 16384.0;
        double az = accelZ / 16384.0;

        printf("Accelerometer X: %f g, Y: %f g, Z: %f g\n", ax, ay, az);

        // Map ax value to servo angle and move the servo
        sg90.move(mapAxToServoAngle(ax));
        
        sleep(1); // Delay for 1 second
    }

    i2cClose(handle);
    return 0;
}

