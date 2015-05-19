#ifndef PM_SENSOR_PARAM_H
#define PM_SENSOR_PARAM_H

enum ParticleType {SmallParticle=0, LargeParticle, NumParticleType};

struct PMSensorParam
{
    int smallParticlePin;
    int largeParticlePin;
    float sensitivity;
    unsigned int sampleWindowDt; // ms
};

#endif
