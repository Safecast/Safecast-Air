#include "logger.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GPS.h>
#include <Adafruit_SHT31.h>
#include "constants.h"
#include "configuration.h"
#include "opcn2_ids.h"

extern Configuration configuration;
extern GPSMonitor gpsMonitor;
extern OPCN2 particleCounter;
extern Adafruit_SSD1306 display;
extern Adafruit_SHT31 humidityAndTempSensor; 

Logger::Logger(LoggerParam param, Openlog &openlog) : openlog_(openlog)
{
    param_ = param;
}


bool Logger::initializeFile()
{
    return openlog_.openNewLogFile();
}

bool Logger::initializeWifi()
{ 
    wifiOK_ = true;
    wifiErrorMsg_ = String("");

    pinMode(constants::DefaultWifiParam.pwrPin, OUTPUT);
    wifiTogglePower();

    wifi_ = ESP8266(*constants::DefaultWifiParam.serialPtr, constants::DefaultWifiParam.baudRate);

    if (!wifi_.setOprToStation()) 
    {
        wifiErrorMsg_ = String("err stat");
        wifiOK_= false;
        return wifiOK_;
    }

    if (!wifi_.joinAP(configuration.wifiSSID(), configuration.wifiPass()))
    {
        wifiErrorMsg_ = String("err join");
        wifiOK_= false;
        return wifiOK_;
    }

    if (!wifi_.disableMUX()) 
    {
        wifiErrorMsg_ = String("err mux");
        wifiOK_= false;
        return wifiOK_;
    }

    wifiIP_ = wifi_.getLocalIP();
    return wifiOK_; 
}


bool Logger::wifiOK()
{
    return wifiOK_;
}


void Logger::wifiPowerOn()
{
    digitalWrite(constants::DefaultWifiParam.pwrPin, LOW);
}

void Logger::wifiPowerOff()
{ 
    digitalWrite(constants::DefaultWifiParam.pwrPin, HIGH);
}


void Logger::wifiTogglePower()
{
    wifiPowerOff();
    delay(constants::DefaultWifiParam.pwrOffDelay);
    wifiPowerOn();
    delay(constants::DefaultWifiParam.pwrOnDelay);
}


String Logger::wifiIP()
{
    return wifiIP_;
}


String Logger::wifiErrorMsg()
{
    return wifiErrorMsg_;
}


unsigned long Logger::logWritePeriod()
{
    return param_.logWritePeriod;
}


unsigned long Logger::dataSamplePeriod()
{
    return param_.dataSamplePeriod;
}

void Logger::writeLogOnTimer()
{
    writeLogFlag_ = true;
}


void Logger::dataSampleOnTimer()
{
    dataSampleFlag_ = true;
}


void Logger::update()
{
    if (dataSampleFlag_)
    {
        opcn2Data_ = particleCounter.getHistogramData();
        humidity_ = humidityAndTempSensor.readHumidity(); 
        temperature_ =  humidityAndTempSensor.readTemperature();
        writeDisplay();
        dataSampleFlag_ = false;
    }
    if (writeLogFlag_)
    {
        writeLog();
        writeLogFlag_ = false;
        wifiSendDataToServer();
    }
}


void Logger::writeLog()
{
    OPCN2Ids opcn2Ids = particleCounter.getIds();

    // Get GPS position data
    gpsDataOk_ = false;
    if (gpsMonitor.haveData())
    {
        gpsData_ = gpsMonitor.getData(&gpsDataOk_);
    }
    if (!gpsDataOk_)
    {
        return;
    }
    String dateTimeString = gpsData_.getDateTimeString();
    String latitudeString = gpsData_.getLatitudeString();
    String longitudeString = gpsData_.getLongitudeString();

    // Create Json output data
    jsonBuffer_ = StaticJsonBuffer<JsonBufferSize>(); // Clear the buffer 
    JsonObject &rootObj = jsonBuffer_.createObject();
    rootObj["dev"] = constants::DeviceName.c_str();        
    String deviceId = configuration.deviceId();
    rootObj["id"]  = deviceId.c_str();              
    rootObj["msg"] = "dat";

    // Add GPS data
    JsonObject &gpsObj = rootObj.createNestedObject("gps");
    gpsObj["date"] = dateTimeString.c_str();
    gpsObj["lat"] = latitudeString.c_str();
    gpsObj["lon"] = longitudeString.c_str();
    gpsObj["alt"] = gpsData_.getAltitudeInMeter();
    gpsObj["spd"] = gpsData_.getSpeedInMeterPerSec();
    gpsObj["ang"] = gpsData_.angle;
    gpsObj["fix"] = gpsData_.fix;
    gpsObj["num"] = gpsData_.satellites;

    // Add OPN2 sensor  
    {
        JsonArray &pmArray = rootObj.createNestedArray("pm");
        JsonObject &opcn2Obj = pmArray.createNestedObject();
        opcn2Obj["pos"] = "norm";
        opcn2Obj["pm1"] = opcn2Data_.PM1;
        opcn2Obj["pm2_5"] = opcn2Data_.PM2_5;
        opcn2Obj["pm10"] = opcn2Data_.PM10;
        opcn2Obj["rate"] = opcn2Data_.flowRate;
        opcn2Obj["dt"] = opcn2Data_.samplePeriod;
        opcn2Obj["mtof1"] = opcn2Data_.bin1_mtof;
        opcn2Obj["mtof3"] = opcn2Data_.bin3_mtof;
        opcn2Obj["mtof5"] = opcn2Data_.bin5_mtof;
        opcn2Obj["mtof7"] = opcn2Data_.bin7_mtof;
        JsonArray &binCntArray = opcn2Obj.createNestedArray("cnt");
        for (int i=0; i<OPCN2Data::NumHistogramBins; i++)
        {
            binCntArray.add(opcn2Data_.binCount[i]);
        }

        JsonObject &idsObj = opcn2Obj.createNestedObject("ids");
        idsObj["pm1"] = opcn2Ids.pm1;
        idsObj["pm2_5"] = opcn2Ids.pm2_5;
        idsObj["pm10"] = opcn2Ids.pm10;
        idsObj["rate"] = opcn2Ids.rate;
        idsObj["dt"] = opcn2Ids.dt;
        idsObj["mtof1"] = opcn2Ids.mtof1;
        idsObj["mtof3"] = opcn2Ids.mtof3;
        idsObj["mtof5"] = opcn2Ids.mtof5;
        idsObj["mtof7"] = opcn2Ids.mtof7;
        JsonArray &cntIdsArray = idsObj.createNestedArray("cnt");
        for (int i=0; i<OPCN2Data::NumHistogramBins; i++)
        {
            cntIdsArray.add(opcn2Ids.cnt[i]);
        }
    }

    // Add temperature humidity sensor
    JsonObject &sht31Obj = rootObj.createNestedObject("sht31");
    sht31Obj["temp"] = temperature_;
    sht31Obj["humd"] = humidity_;
    JsonObject &sht31IdsObj = sht31Obj.createNestedObject("ids");
    sht31IdsObj["temp"] = configuration.sht31Ids().temperature; 
    sht31IdsObj["humd"] = configuration.sht31Ids().humidity;

    // Write json object to open log
    rootObj.printTo(*openlog_.getSerialPtr());
    openlog_.println();
}


void Logger::writeDisplay() 
{
    // Display the data
    const int col2 = DisplayCol2;
    const int col3 = DisplayCol3;

    SPI.beginTransaction(constants::DisplaySPISettings);            
    display.clearDisplay();
    display.setCursor(0,0);

    display.print("PM1");
    display.setCursor(col2,display.getCursorY());
    display.print(opcn2Data_.PM1);
    display.setCursor(col3,display.getCursorY());
    display.println("ug/m3");

    display.print("PM2.5");
    display.setCursor(col2,display.getCursorY());
    display.print(opcn2Data_.PM2_5);
    display.setCursor(col3,display.getCursorY());
    display.println("ug/m3");

    display.print("PM10");
    display.setCursor(col2,display.getCursorY());
    display.print(opcn2Data_.PM10);
    display.setCursor(col3,display.getCursorY());
    display.println("ug/m3");

    char tmpStr[50];

    display.print("TEMP");
    display.setCursor(col2,display.getCursorY());
    snprintf(tmpStr,sizeof(tmpStr),"%1.2f",temperature_);
    display.print(tmpStr);
    display.setCursor(col3,display.getCursorY());
    display.println("C");

    display.print("HUMD");
    display.setCursor(col2,display.getCursorY());
    snprintf(tmpStr,sizeof(tmpStr), "%1.2f",humidity_);
    display.print(tmpStr);
    display.setCursor(col3,display.getCursorY());
    display.println("%");

    display.print("FIX");
    display.setCursor(col2,display.getCursorY());
    if ((gpsData_.fix) && (gpsDataOk_))
    {
        display.println("true");
    }
    else
    {
        display.println("false");
    }
    display.print("WIFI");
    display.setCursor(col2,display.getCursorY());
    if (wifiOK_)
    {
        display.print("ok");
        display.setCursor(col3,display.getCursorY());
        display.print(wifiSendCount_);
    }
    else
    {
        display.println(wifiErrorMsg_);
    }

    display.display();

    SPI.endTransaction();
}


bool Logger::wifiReset()
{
    initializeWifi();
    wifiResetCount_++;
    bool rval = true;
    if (wifiOK_)
    {
        wifiSendFailCount_ = 0;
    }
    else
    {
        rval = false;
    }
    return rval;
}


void Logger::wifiSendDataToServer() 
{
    Serial.println("wifiSendDataToServer");

    if (!wifiOK_) 
    {
        wifiReset();
    }
    Serial.println("wifi OK");

    if (!gpsDataOk_)
    {
        return;
    }
    Serial.println("gpsData OK");
    Serial.println();

    String captured_at = gpsData_.getDateTimeString();
    String deviceId  = configuration.deviceId();
    String dateTime  = gpsData_.getDateTimeString();
    String latitude  = gpsData_.getLatitudeString();
    String longitude = gpsData_.getLongitudeString();
    String height    = String(gpsData_.getAltitudeInMeter(),5); 

    String PM1_value   = String(opcn2Data_.PM1,5);
    String PM2_5_value = String(opcn2Data_.PM2_5,5);
    String PM10_value  = String(opcn2Data_.PM10,5);
    String temp_value  = String(temperature_,5);
    String humd_value  = String(humidity_,5);

    String PM1_unit   = String("PM1");
    String PM2_5_unit = String("PM2.5");
    String PM10_unit  = String("PM10");
    String temp_unit  = String("TEMPC");
    String humd_unit  = String("HUMD%");

    String opcn2Type = String("OPCN2");
    String sht31Type = String("SHT31");

    OPCN2Ids opcn2Ids = particleCounter.getIds();
    unsigned int temp_id = configuration.sht31Ids().temperature; 
    unsigned int humd_id = configuration.sht31Ids().humidity;

    String unitArray[WifiNumUnitToSend] = {PM1_unit, PM2_5_unit, PM10_unit, temp_unit, humd_unit};
    String valueArray[WifiNumUnitToSend] = {PM1_value, PM2_5_value, PM10_value, temp_value, humd_value};
    String deviceTypeIdArray[WifiNumUnitToSend] = {opcn2Type, opcn2Type, opcn2Type, sht31Type, sht31Type};
    unsigned int sensorIdArray[WifiNumUnitToSend] = {opcn2Ids.pm1, opcn2Ids.pm2_5, opcn2Ids.pm10, temp_id, humd_id}; 

    String hostname = configuration.randomGateway();
    unsigned int  port = configuration.port();
    String apiKey = configuration.apiKey();

    for (int i=0; i<WifiNumUnitToSend; i++)
    { 

        if (wifiSendFailCount_ >= WifiMaxSendFailCount) 
        { 
            wifiReset();
        }
        if (!wifiOK_)
        {
            continue;
        }

        jsonBuffer_ = StaticJsonBuffer<JsonBufferSize>(); // Clear the buffer 
        JsonObject &testObj = jsonBuffer_.createObject();
        testObj["captured_at"] = captured_at.c_str(); 
        testObj["longitude"] =  longitude.c_str();  
        testObj["latitude"] = latitude.c_str();
        testObj["device_id"] = deviceId.c_str();
        testObj["value"] = valueArray[i].c_str();
        testObj["unit"] = unitArray[i].c_str();
        testObj["height"] = height.c_str();;
        testObj["devicetype_id"] = deviceTypeIdArray[i].c_str();
        testObj["sensor_id"] = sensorIdArray[i];

        char jsonCharBuffer[JsonBufferSize];
        testObj.printTo(jsonCharBuffer,JsonBufferSize);
        String jsonCharBufferLen = String(strlen(jsonCharBuffer));

        String sendString;
        sendString += String("POST ");
        if (apiKey.length() == 0)
        {
            sendString += String("/jsontest ");
        }
        else
        {
            sendString += String("/scripts/indextest.php?api_key=") + apiKey + String(" ");
        }
        sendString += String("HTTP/1.1\r\n");
        sendString += String("Connection: close\r\n");
        sendString += String("Host: ") + hostname + String(":") + String(port) + String("\r\n");
        sendString += String("User-Agent: Arduino\r\n");
        sendString += String("Content-Type: application/json\r\n");
        sendString += String("Content-Length: ") + String(jsonCharBufferLen)+ String("\r\n\r\n");
        sendString += String(jsonCharBuffer);
        sendString += String("\r\n");
        const char *sendBuffer = sendString.c_str();

        Serial.print("cnt:   ");
        Serial.println(wifiSendCount_);
        Serial.print("fail:  ");
        Serial.println(wifiSendFailCount_);
        Serial.print("reset: ");
        Serial.println(wifiResetCount_);
        Serial.println();
        Serial.print(sendBuffer);
        Serial.println();
        Serial.print("connect to host: ");

        if (wifi_.createTCP(hostname, port))
        {
            Serial.println("ok");

            wifi_.send((const uint8_t *)sendBuffer, strlen(sendBuffer));

            uint8_t recvBuffer[1024] = {0};
            uint32_t len = wifi_.recv(recvBuffer, sizeof(recvBuffer), 2000);
            if (len > 0) 
            {
                Serial.print("received:");
                for(uint32_t i = 0; i < len; i++) 
                {
                    Serial.print((char)recvBuffer[i]);
                }
                wifiSendFailCount_ = 0;
            }
            wifiSendCount_++;
        } 
        else 
        {
            Serial.println("err");
            wifiSendFailCount_++;
        }
        Serial.println();
        delay(500);
    }
}
