#ifndef LOGGER_H
#define LOGGER_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266.h>
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
        static const int WifiNumUnitToSend = 5;
        static const int WifiMaxSendFailCount = 2;
        
        Logger(LoggerParam param, Openlog &openlog);

        bool initializeFile();
        bool initializeWifi();

        unsigned long logWritePeriod();
        unsigned long dataSamplePeriod();

        void writeLogOnTimer();
        void dataSampleOnTimer();
        void update();

        void writeLog();
        void writeDisplay(); 

        bool wifiOK();
        String wifiIP();
        String wifiErrorMsg();
        bool wifiReset();
        void wifiPowerOn();
        void wifiPowerOff();
        void wifiTogglePower();
        void wifiSendDataToServer(); 

    protected:

        Openlog &openlog_;
        ESP8266 wifi_; 

        bool wifiOK_ = false;;
        String wifiIP_;
        String wifiErrorMsg_;
        uint32_t wifiSendCount_ = 0;      // Number of wifi message sent
        uint32_t wifiSendFailCount_ = 0;  // Number of consecutive send failures 
        uint32_t wifiResetCount_ = 0;     // Number of wifi resets
        uint32_t wifiSendUnitIndex_ = 0;  // Unit index for wifi sends

        LoggerParam param_;
        StaticJsonBuffer<JsonBufferSize> jsonBuffer_;

        bool gpsDataOk_ = false;
        GPSData gpsData_;

        OPCN2Data opcn2Data_;
        volatile float temperature_;  // deg C
        volatile float humidity_;     // percent
        volatile bool writeLogFlag_ = false;
        volatile bool dataSampleFlag_ = false;

};

#endif

