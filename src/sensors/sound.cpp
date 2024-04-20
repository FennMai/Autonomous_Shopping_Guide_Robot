#include "sound.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <pigpio.h>
#include <cstring>
#include <functional> 

SoundManager::SoundManager(int i2cBus, uint8_t deviceAddress)
    : device(i2cBus, deviceAddress), continueChecking(true) {
    if (gpioInitialise() < 0) {
        throw std::runtime_error("Failed to initialize GPIO");
    }
    if (!device.begin()) {
        gpioTerminate();
        throw std::runtime_error("Failed to start communication with the device");
    }

    // Set up shared memory
    shm = shared_memory_object(open_or_create, "CmdIdSharedMemory", read_write);
    shm.truncate(sizeof(uint8_t));
    region = mapped_region(shm, read_write);
}

SoundManager::~SoundManager() {
    continueChecking = false;  // Stop the checkCMDID loop
    shared_memory_object::remove("CmdIdSharedMemory");
    gpioTerminate();
}

void SoundManager::startCheckingCMDID() {
    std::thread([this] { this->checkCMDID(); }).detach();
}

void SoundManager::checkCMDID() {
    while (continueChecking) {
        uint8_t CMDID = device.getCMDID();
        if (CMDID != 0) {
            std::cout << "CMDID = " << static_cast<int>(CMDID) << std::endl;
            std::memcpy(region.get_address(), &CMDID, sizeof(CMDID));
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

//void SoundManager::updateOutputText(const std::function<void(const std::string&)> &updateFunc) {
//    uint8_t CMDID = 0;
//    memcpy(&CMDID, region.get_address(), sizeof(uint8_t));  // Read CMDID from shared memory
//    updateFunc("CMDID: " + std::to_string(CMDID));

//}

void SoundManager::updateOutputText(const std::function<void(int)> &updateFunc) {
    uint8_t CMDID = 0;
    memcpy(&CMDID, region.get_address(), sizeof(uint8_t));  // Read CMDID from shared memory
    updateFunc(static_cast<int>(CMDID));  // 直接传递整型值
}


void SoundManager::playCommand(uint8_t CMDID) {
    device.playByCMDID(CMDID);
}
