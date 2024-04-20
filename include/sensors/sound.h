#ifndef SOUND_H
#define SOUND_H

#include "DFbot_voi.h"
#include <cstdint>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <functional> 

using namespace boost::interprocess;

class SoundManager {
public:
    SoundManager(int i2cBus, uint8_t deviceAddress);
    ~SoundManager();

    void startCheckingCMDID();
//    void updateOutputText(const std::function<void(const std::string&)> &updateFunc);
    void updateOutputText(const std::function<void(int)> &updateFunc);
    void playCommand(uint8_t CMDID);

private:
    DFRobot_DF2301Q_RPi device;
    shared_memory_object shm;
    mapped_region region;
    bool continueChecking;

    void checkCMDID();
};

#endif // SOUND_H
