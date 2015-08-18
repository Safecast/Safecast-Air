#ifndef LOGGER_H
#define LOGGER_H
#include "logger_param.h"
#include <Arduino.h>

class Logger
{
    public:
        static const uint8_t DefaultTimerPriority = 124;
        static const int JsonBufferSize = 3000;

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

        static void dummyTimerCallback() { }; 
        void (*timerCallback_)() = dummyTimerCallback; 

};

#endif
