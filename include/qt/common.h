#ifndef COMMON_H
#define COMMON_H

struct LidarPoint {
    float phi;
    float r;
    float x;
    float y;
    unsigned int signal_strength;
    bool valid;
};

#endif // COMMON_H
