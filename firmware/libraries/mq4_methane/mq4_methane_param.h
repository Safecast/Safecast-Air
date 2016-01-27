#ifndef MQ4_METHANE_PARAM_H
#define MQ4_METHANE_PARAM_H

const int MQ4_MaxNumLookup =10;

struct MQ4_MethanIds
{
    unsigned int ain;
    unsigned int ppm;
    unsigned int ppmFlt;
};

struct MQ4_MethaneParam
{
    uint8_t priority;
    unsigned long sampleDt;

    int ainPin;
    int ainReadRes;
    int ainReadAvg;
    float ainMaxVal;

    float lowPassCutoffFreq;
    unsigned int lowPassOrder;

    int  numLookup;
    float tableVoltToPPM[MQ4_MaxNumLookup][2]; 

    MQ4_MethanIds ids;

};

#endif
