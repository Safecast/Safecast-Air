#ifndef LOGGER_H
#define LOGGER_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFlyHQ.h>
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
        static const int NumUnitToSend = 5;
        static const int MaxSendFailCount = 2*NumUnitToSend - 1;
        static const int PwrInitDelay = 1000;
        
        Logger(LoggerParam param, Openlog &openlog);
        bool initializeFile();
        bool initializeWiFly();
        void initializeWiFlyPwrPin();
        void toggleWiFlyPwr();

        unsigned long logWritePeriod();
        unsigned long dataSamplePeriod();

        void writeLogOnTimer();
        void dataSampleOnTimer();
        void update();

        void writeLog();
        void writeDisplay(); 

        String ip();
        bool haveWiFly();
        bool haveNetwork();
        void sendDataToServer(); 

    protected:

        Openlog &openlog_;

        rnxv::WiFly wiFly_;
        bool haveWiFly_ = false;
        bool haveNetwork_ = false;
        int sendFailCount_ = 0;
        int wiflyRebootCount_ = 0;
        String ip_;

        LoggerParam param_;
        volatile bool writeLogFlag_ = false;
        volatile bool dataSampleFlag_ = false;
        unsigned long count_ = 0;
        StaticJsonBuffer<JsonBufferSize> jsonBuffer_;

        bool gpsDataOk_ = false;
        GPSData gpsData_;
        OPCN2Data opcn2Data_;
        float temperature_;  // deg C
        float humidity_;     // percent

};

#endif

