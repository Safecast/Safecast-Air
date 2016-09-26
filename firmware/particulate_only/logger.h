#ifndef LOGGER_H
#define LOGGER_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "logger_param.h"
#include "gps_monitor.h"
#include "opcn2.h"
#include "openlog.h"

class Logger
{
    public:
        static const int JsonBufferSize = 6000;  // Probably bigger than we need
        static const int DisplayCol2 = 50;
        static const int DisplayCol3 = 90;
        
        Logger(LoggerParam param, Openlog &openlog);
        bool initialize();

        unsigned long logWritePeriod();
        unsigned long dataSamplePeriod();
        void writeLogOnTimer();
        void dataSampleOnTimer();
        void update();

        void writeLog();
        void writeDisplay(); 

    protected:
        Openlog &openlog_;

        LoggerParam param_;
        volatile bool writeLogFlag_ = false;
        volatile bool dataSampleFlag_ = false;
        unsigned long count_ = 0;
        StaticJsonBuffer<JsonBufferSize> jsonBuffer_;

        GPSData gpsData_;
        OPCN2Data opcn2Data_;
        float temperature_;  // deg C
        float humidity_;     // percent

};

#endif

