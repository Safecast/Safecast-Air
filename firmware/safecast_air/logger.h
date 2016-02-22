#ifndef LOGGER_H
#define LOGGER_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "logger_param.h"
#include "opcn2.h"
#include "gps_monitor.h"

class Logger
{
    public:
        //static const uint8_t DefaultTimerPriority = 124;
        static const int JsonBufferSize = 6000;  // Probably bigger than we need

        Logger(LoggerParam param, HardwareSerial *serialPtr=&Serial3);

        void initialize();
        unsigned long period();
        void onTimer();
        void update();

        void writeConfiguration();
        void writeData();
        void writeDisplay(); // Temporary hack

    protected:
        HardwareSerial *serialPtr_;
        LoggerParam param_;
        volatile bool writeDataFlag_ = false;
        unsigned long count_ = 0;
        StaticJsonBuffer<JsonBufferSize> jsonBuffer_;

        GPSData gpsData_;
        OPCN2Data opcn2Data_;

};

#endif

