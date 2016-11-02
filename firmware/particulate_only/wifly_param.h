#ifndef WIFLY_PARAM_H
#define WIFLY_PARAM_H
#include <Arduino.h>

struct WiflyParam
{
    int pwrPin;
    unsigned long pwrOnDelay;
    unsigned long pwrOffDelay;
    unsigned long baudRate; 
    HardwareSerial *serialPtr;
};

#endif
