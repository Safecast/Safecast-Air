#include "configuration.h"
#include "constants.h"

Configuration::Configuration(String filename, Openlog &openlog) 
: filename_(filename), openlog_(openlog)
{ }


Configuration::Configuration(char filename[], Openlog &openlog) 
: filename_(filename), openlog_(openlog)
{ }


bool Configuration::initialize()
{
    // Set random seed for random gatway selection
    randomSeed(analogRead(constants::RandomSeedAinPin));  

    openlog_.flush();

    // Read configuration file contents and parse json
    String fileContents = openlog_.readFile(filename_);
    if (fileContents.length() == 0)
    {
        return false;
    }
    StaticJsonBuffer<JsonBufferSize> jsonBuffer;
    JsonObject &jsonRoot  = jsonBuffer.parseObject((char *)(fileContents.c_str()));
    if (!jsonRoot.success())
    {
        return false;
    }

    // Extract configuration data from json
    return setConfigFromJson(jsonRoot);
} 


bool Configuration::setConfigFromJson(JsonObject &jsonRoot)
{
    if (jsonRoot.containsKey("username"))
    {
        const char *usernameTmp = jsonRoot["username"];
        username_ = String(usernameTmp);
    }
    else
    {
        errorMsg_ = String("no username");
        return false;
    }

    if (jsonRoot.containsKey("device_id"))
    {
        const char *deviceIdTmp = jsonRoot["device_id"];
        deviceId_= String(deviceIdTmp);
    }
    else
    {
        errorMsg_ = String("no device_id");
        return false;
    }

    if (jsonRoot.containsKey("wifi_ssid"))
    {
        const char *wifiSSIDTmp = jsonRoot["wifi_ssid"];
        wifiSSID_ = String(wifiSSIDTmp);
    }
    else
    {
        errorMsg_ = String("no wifi_ssid");
        return false;
    }

    if (jsonRoot.containsKey("wifi_pass"))
    {
        const char *wifiPassTmp = jsonRoot["wifi_pass"];
        wifiPass_ = String(wifiPassTmp);
    }
    else
    {
        errorMsg_  = String("no wifi_pass");
        return false;
    }

    if (jsonRoot.containsKey("api_key"))
    {
        const char *apiKeyTmp = jsonRoot["api_key"];
        apiKey_ = String(apiKeyTmp);
    }
    else
    {
        errorMsg_ = String("no api_key");
        return false;
    }

    if (jsonRoot.containsKey("gateways"))
    {
        JsonArray &gatewaysJson = jsonRoot["gateways"];
        if (gatewaysJson.size() == 0)
        {
            errorMsg_ = String("zero gateways");
            return false;
        }
        numberOfGateways_ = min(gatewaysJson.size(),MaxNumberOfGateways);
        for (int i=0; i<numberOfGateways_; i++)
        {
            const char *gatewayTmp = gatewaysJson[i];
            gateways_[i] = String(gatewayTmp);
        }
    }
    else
    {
        errorMsg_ = String("no gateways");
        return false;
    }

    if (jsonRoot.containsKey("port"))
    {
        port_ = jsonRoot["port"];
    }

    if (jsonRoot.containsKey("opcn2_ids"))
    {
        JsonArray &dataIdsJson = jsonRoot["opcn2_ids"];
        if (dataIdsJson.size() != OPCN2Ids::NumDataIds+1)
        {
            errorMsg_ = String("err: opcn2 dids") + String(OPCN2Ids::NumDataIds+1);
            return false;
        }
        opcn2Ids_.pm1 = dataIdsJson[0];
        opcn2Ids_.pm2_5 = dataIdsJson[1];
        opcn2Ids_.pm10 = dataIdsJson[2];
        opcn2Ids_.rate = dataIdsJson[3];
        opcn2Ids_.dt = dataIdsJson[4];
        opcn2Ids_.mtof1 = dataIdsJson[5];
        opcn2Ids_.mtof3 = dataIdsJson[6];
        opcn2Ids_.mtof5 = dataIdsJson[7];
        opcn2Ids_.mtof7 = dataIdsJson[8];

        JsonArray &histIdsJson = dataIdsJson[OPCN2Ids::IndexHistogramIds];
        if (histIdsJson.size() != OPCN2Ids::NumHistogramIds)
        {
            errorMsg_ = String("err: opcn2 hids");
            return false;
        }
        for (int i=0; i<OPCN2Ids::NumHistogramIds; i++)
        {
            opcn2Ids_.cnt[i] = histIdsJson[i];
        }
    }
    else
    {
        errorMsg_ = String("no opcn2_ids");
        return false;
    }

    if (jsonRoot.containsKey("sht31_ids"))
    {
        JsonArray &sht31Array = jsonRoot["sht31_ids"]; 
        if (sht31Array.size() != 2)
        {
            errorMsg_ = String("err: #sht31_ids");
            return false;
        }
        sht31Ids_.temperature = sht31Array[SHT31Ids::IndexTemperature];
        sht31Ids_.humidity = sht31Array[SHT31Ids::IndexHumidity];
    }
    else
    {
        errorMsg_ = String("no sht31_ids");
        return false;
    }

    isInitialized_= true;
    return true;
}

bool Configuration::isInitialized()
{
    return isInitialized_;
}


String Configuration::username()
{
    return username_;
}


String Configuration::deviceId()
{
    return deviceId_;
}


String Configuration::wifiSSID()
{
    return wifiSSID_;
}


String Configuration::wifiPass()
{
    return wifiPass_;
}


String Configuration::apiKey()
{
    return apiKey_;
}


int Configuration::numberOfGateways()
{
    return numberOfGateways_;
}


String Configuration::gateway(int num)
{
    if ((num > 0) && (num < numberOfGateways_))
    {
        return gateways_[num];
    }
    else
    {
        return String("");
    }
}


String Configuration::randomGateway()
{
    int num = random(0,numberOfGateways_);
    return gateways_[num];
}


unsigned int Configuration::port()
{
    return port_;
}


OPCN2Ids Configuration::opcn2Ids()
{
    return opcn2Ids_;
}

SHT31Ids Configuration::sht31Ids()
{
    return sht31Ids_;
}

void Configuration::setUsername(String username)
{
    username_ = username;
}


void Configuration::setDeviceId(String deviceId)
{
    deviceId_ = deviceId;
}


void Configuration::setWifiSSID(String wifiSSID)
{
    wifiSSID_ = wifiSSID;
}


void Configuration::setWifiPass(String wifiPass)
{
    wifiPass_ = wifiPass;
}


void Configuration::setApiKey(String apiKey)
{
    apiKey_ = apiKey;
}


void Configuration::setNumberOfGateways(int num)
{
    numberOfGateways_ = num;
}


void Configuration::setGateway(int num, String gateway)
{
    if ((num > 0) && (num < numberOfGateways_))
    {
        gateways_[num] = gateway;
    }
}


void Configuration::setPort(unsigned int port)
{
    port_ = port;
}


void Configuration::setOPCN2Ids(OPCN2Ids opcn2Ids)
{
    opcn2Ids_ = opcn2Ids;
}


void Configuration::setSHT31Ids(SHT31Ids sht31Ids)
{
    sht31Ids_ = sht31Ids;
}


String Configuration::errorMsg()
{
    return errorMsg_;
}

String Configuration::toString()
{
    //
    // Get configuration as string - for debugging/testing
    //
    String configString = String("");
    configString += String("filename: ") + filename_ + String("\n");
    configString += String("username: ") + username_ + String("\n");
    configString += String("deviceId: ") + deviceId_ + String("\n");
    configString += String("wifiSSID: ") + wifiSSID_ + String("\n");
    configString += String("wifiPass: ") + wifiPass_ + String("\n");
    configString += String("apiKey:   ") + apiKey_   + String("\n"); 
    configString += String("gateways: [");
    for (int i=0; i<numberOfGateways_; i++)
    {
        configString += gateways_[i];
        if (i < (numberOfGateways_-1))
        {
            configString += String(",");
        }
    }
    configString += String("]\n"); 
    configString += String("opcn2Ids: [");
    configString += String(opcn2Ids_.pm1)   + String(",");
    configString += String(opcn2Ids_.pm2_5) + String(",");
    configString += String(opcn2Ids_.pm10)  + String(",");
    configString += String(opcn2Ids_.rate)  + String(",");
    configString += String(opcn2Ids_.dt)    + String(",");
    configString += String(opcn2Ids_.mtof1) + String(",");
    configString += String(opcn2Ids_.mtof3) + String(",");
    configString += String(opcn2Ids_.mtof5) + String(",");
    configString += String(opcn2Ids_.mtof7) + String(",");
    configString += String("[");
    for (int i=0; i<OPCN2Ids::NumHistogramIds; i++)
    {
        configString += String(opcn2Ids_.cnt[i]);
        if (i<OPCN2Ids::NumHistogramIds-1)
        {
            configString += String(",");
        }
    }
    configString += String("]]\n");
    configString += String("sht31Ids: [");
    if (SHT31Ids::IndexTemperature < SHT31Ids::IndexHumidity)
    {
        configString += sht31Ids_.temperature;
        configString += String(",");
        configString += sht31Ids_.humidity; 
    }
    else
    {
        configString += sht31Ids_.humidity; 
        configString += String(",");
        configString += sht31Ids_.temperature;
    }
    configString += String("]\n");
    return configString;
}
