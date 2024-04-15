#include <pigpio.h>
#include <iostream>
#include <unistd.h> 

class Buzzer {
public:
    Buzzer(int pin) : pin_(pin) {
        if (gpioInitialise() < 0) {
            std::cerr << "Failed to initialize GPIO\n";
            exit(1);
        }
        gpioSetMode(pin_, PI_OUTPUT);
        gpioWrite(pin_, 1); // setup initial to off state
    }

    ~Buzzer() {
        gpioWrite(pin_, 1); // insure gpio is off
        gpioTerminate(); // clean
    }

    void dot() {
        gpioWrite(pin_, 0); // start
        usleep(200000); // been 20000ms
        gpioWrite(pin_, 1); // stop
        usleep(200000); // gap stop for next
    }

    void dash() {
        gpioWrite(pin_, 0); 
        usleep(600000); 
        gpioWrite(pin_, 1); 
        usleep(200000); 
    }

    void playMorseCode(const std::string& code) {
        for (char c : code) {
            if (c == '.') {
                dot();
            } else if (c == '-') {
                dash();
            }
        }
        usleep(600000); 
    }

private:
    int pin_;
};

int main() {
    Buzzer buzzer(25); // gpio pin

    // mos code
    std::string morseCode[] = {"-..", "---", "-.-.", "-", "---", ".-."};

    for (const std::string& code : morseCode) {
        buzzer.playMorseCode(code);
        usleep(1400000); //gep stop for every single word
    }

    return 0;
}

