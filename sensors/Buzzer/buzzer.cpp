#include <pigpio.h>
#include <iostream>

class Buzzer {
public:
    Buzzer(int pin) : pin_(pin) {
        if (gpioInitialise() < 0) {
            std::cerr << "Failed to initialize GPIO\n";
            exit(1);
        }
        gpioSetMode(pin_, PI_OUTPUT); 
        gpioWrite(pin_, 1); 
    }

    ~Buzzer() {
        gpioTerminate(); 
    }

    void start() {
        gpioWrite(pin_, 0); 
    }

    void stop() {
        gpioWrite(pin_, 1); 
    }

private:
    int pin_;
};

int main() {
    Buzzer buzzer(18); 

    buzzer.start(); 
    time_sleep(1); 
    buzzer.stop(); 

    return 0;
}

