#ifndef LOGGER_H
#define LOGGER_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "logger_param.h"

class Logger
{
    public:
        static const uint8_t DefaultTimerPriority = 124;
        static const int JsonBufferSize = 5000;  // Probably bigger than we need

        Logger(LoggerParam param, HardwareSerial *serialPtr=&Serial3);

        void initialize();

        void start();
        void stop();
        void setTimerCallback(void (*timerCallback)());
        void onTimer();
        void update();

        void writeConfiguration();
        void writeData();

    protected:
        HardwareSerial *serialPtr_;
        IntervalTimer timer_;
        LoggerParam param_;
        volatile bool writeDataFlag_ = false;
        unsigned long count_ = 0;
        StaticJsonBuffer<JsonBufferSize> jsonBuffer_;

        static void dummyTimerCallback() { }; 
        void (*timerCallback_)() = dummyTimerCallback; 

};

#endif
