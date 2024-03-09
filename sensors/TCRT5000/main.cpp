#include <iostream>
#include <pigpio.h>
#include "TCRT5000_Drive.h"

int main()
{
    constexpr int TCRT5000_DO_PIN = 18;

    TCRT5000 sensor(TCRT5000_DO_PIN);
    sensor.readAndPrint();

    return 0;
}
