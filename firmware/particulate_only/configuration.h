#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "openlog.h"
#include "opcn2_ids.h"
#include "sht31_ids.h"


class Configuration
{
    public:
        static const int JsonBufferSize = 1000;

        Configuration(String filename, Openlog &openlog);
        Configuration(char filename[], Openlog &openlog);

        bool initialize();
        bool isInitialized();

        bool setConfigFromJson(JsonObject &jsonRoot);

        String username();
        String deviceId();
        String wifiSSID();
        String wifiPass();
        OPCN2Ids opcn2Ids();
        SHT31Ids sht31Ids();

        void setUsername(String username);
        void setDeviceId(String deviceId);
        void setWifiSSID(String wifiSSID);
        void setWifiPass(String wifiPass);
        void setOPCN2Ids(OPCN2Ids opcn2Ids);
        void setSHT31Ids(SHT31Ids sht31Ids);

        String errorMsg();
        String toString();

       
    protected:
        bool isInitialized_ = false;
        String filename_ = String("");
        Openlog &openlog_;

        String username_ = String("");
        String deviceId_ = String("");
        String wifiSSID_ = String("");
        String wifiPass_ = String("");
        OPCN2Ids opcn2Ids_; 
        SHT31Ids sht31Ids_;

        String errorMsg_ = String("");


};
            

#endif
