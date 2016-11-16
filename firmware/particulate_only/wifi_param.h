#ifndef WIFI_PARAM_H
#define WIFI_PARAM_H
#include <Arduino.h>

struct WifiParam
{
    int pwrPin;
    unsigned long pwrOnDelay;
    unsigned long pwrOffDelay;
    unsigned long baudRate; 
    HardwareSerial *serialPtr;
};

#endif
