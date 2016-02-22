#ifndef LOGGER_H
#define LOGGER_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "logger_param.h"

class Logger
{
    public:
        static const int JsonBufferSize = 6000;  // Probably bigger than we need
        static const int WiFlyMaxFailCount = 3;

        Logger(LoggerParam param, HardwareSerial *serialPtr=&Serial3);

        void initialize();
        unsigned long period();
        void onTimer();
        void update();

        void writeConfiguration();
        void writeData();

    protected:
        HardwareSerial *serialPtr_;
        LoggerParam param_;
        int wiflyFailCount_;
        volatile bool writeDataFlag_ = false;
        StaticJsonBuffer<JsonBufferSize> jsonBuffer_;

};

#endif
