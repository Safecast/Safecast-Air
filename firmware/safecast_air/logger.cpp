#include "logger.h"
#include <Streaming.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GPS.h>
#include "constants.h"
#include "opcn2.h"
#include "opcn2_ids.h"
#include "gas_sensor.h"
#include "tmp_sensor.h"
#include "gps_monitor.h"

extern GasSensorDevVector<constants::NumGasSensor> gasSensors;
extern TmpSensorDevVector<constants::NumTmpSensor> tmpSensors;
extern GPSMonitor gpsMonitor;
extern OPCN2 particleCounter;
extern Adafruit_SSD1306 display;

Logger::Logger(LoggerParam param, HardwareSerial *serialPtr)
{
    param_ = param;
    serialPtr_ = serialPtr;
}


void Logger::initialize()
{
    serialPtr_ -> begin(param_.baudRate);
}


void Logger::start()
{
    timer_.priority(param_.priority);
    timer_.begin(timerCallback_, param_.period);
}


void Logger::stop()
{
    timer_.end();
}

void Logger::onTimer()
{
    writeDataFlag_ = true;
}


void Logger::update()
{
    if (writeDataFlag_)
    {
        writeData();
        writeDataFlag_ = false;
    }
}


void Logger::setTimerCallback(void (*timerCallback)())
{
    timerCallback_ = timerCallback;
}

void Logger::writeConfiguration()
{
    jsonBuffer_ = StaticJsonBuffer<JsonBufferSize>(); // Clear the buffer 
    JsonObject &rootObj = jsonBuffer_.createObject();
    rootObj["dev"] = constants::DeviceName;  // Change to something from loaded settings
    rootObj["id"] = constants::DeviceId;     // Change to something from loaded settings
    rootObj["msg"] = "cfg";

    // Add gas sensor metadata
    JsonArray &gasArray = rootObj.createNestedArray("gas");

    //rootObj.printTo(*serialPtr_);
    //*serialPtr_ << endl;

    // Dev
    // ----------------------------------------
    //rootObj.prettyPrintTo(Serial);
    //Serial << endl;
    // ----------------------------------------

}


void Logger::writeData()
{
    //Serial << __PRETTY_FUNCTION__ << endl;

    GPSData gpsData; 
    bool gpsDataOk = false;
    if (gpsMonitor.haveData())
    {
        gpsData = gpsMonitor.getData(&gpsDataOk);
    }
    if (!gpsDataOk)
    {
        return;
    }

    OPCN2Data opcn2Data = particleCounter.getHistogramData();
    OPCN2Ids opcn2Ids = particleCounter.getIds();
    String dateTimeString = gpsData.getDateTimeString();
    String latitudeString = gpsData.getLatitudeString();
    String longitudeString = gpsData.getLongitudeString();

    // Create Json output data
    jsonBuffer_ = StaticJsonBuffer<JsonBufferSize>(); // Clear the buffer 
    JsonObject &rootObj = jsonBuffer_.createObject();
    rootObj["dev"] = constants::DeviceName;  // Change to something from loaded settings
    rootObj["id"] = constants::DeviceId;     // Change to something from loaded settings
    rootObj["msg"] = "dat";

    // Add GPS data
    JsonObject &gpsObj = rootObj.createNestedObject("gps");
    gpsObj["date"] = dateTimeString.c_str();
    gpsObj["lat"] = latitudeString.c_str();
    gpsObj["lon"] = longitudeString.c_str();
    gpsObj["alt"] = gpsData.getAltitudeInMeter();
    gpsObj["spd"] = gpsData.getSpeedInMeterPerSec();
    gpsObj["ang"] = gpsData.angle;
    gpsObj["fix"] = gpsData.fix;
    gpsObj["num"] = gpsData.satellites;

    // Add gas sensors
    JsonArray &gasArray = rootObj.createNestedArray("gas");
    String gasSensorType[constants::NumGasSensor];
    int cnt = 0;
    for (auto &sensor : gasSensors)
    {
        if (sensor.isActive())
        {
            JsonObject &sensorObj = gasArray.createNestedObject();
            GasSensorParam param = sensor.param();
            gasSensorType[cnt] = sensor.gasName();
            sensorObj["type"] = gasSensorType[cnt].c_str();
            sensorObj["hdr"] = param.header;
            sensorObj["pos"] = param.position;
            sensorObj["wrk"] = sensor.working();
            sensorObj["aux"] = sensor.auxillary();
            sensorObj["ppb"] = sensor.ppb();
            sensorObj["ppbFlt"] = sensor.ppbLowPass();
            
            JsonObject &idsObj = sensorObj.createNestedObject("ids");
            idsObj["wrk'"] = param.ids.wrk;
            idsObj["aux'"] = param.ids.aux;
            idsObj["ppb'"] = param.ids.ppb;
            idsObj["ppbFlt'"] = param.ids.ppbFlt;

            cnt++;
        }
    }

    // Add tmp sensors
    JsonArray &tmpArray = rootObj.createNestedArray("tmp");
    for (auto &sensor : tmpSensors)
    {
        JsonObject &sensorObj = tmpArray.createNestedObject();
        TmpSensorParam param = sensor.param();
        sensorObj["hdr"] = param.header;
        sensorObj["val"] = sensor.value();
        sensorObj["valFlt"] = sensor.valueLowPass();
        
        JsonObject &idsObj = sensorObj.createNestedObject("ids");
        idsObj["val"] = param.ids.val;
        idsObj["valFlt"] = param.ids.valFlt;
    }

    // Add OPN2 sensor  ...
    // TO DO modifiy to handle multiple opcn2 devices.
    JsonArray &pmArray = rootObj.createNestedArray("pm");

    JsonObject &opcn2Obj = pmArray.createNestedObject();
    opcn2Obj["pos"] = "norm";
    opcn2Obj["pm1"] = opcn2Data.PM1;
    opcn2Obj["pm2_5"] = opcn2Data.PM2_5;
    opcn2Obj["pm10"] = opcn2Data.PM10;
    opcn2Obj["rate"] = opcn2Data.flowRate;
    opcn2Obj["dt"] = opcn2Data.samplePeriod;
    opcn2Obj["mtof1"] = opcn2Data.bin1_mtof;
    opcn2Obj["mtof3"] = opcn2Data.bin3_mtof;
    opcn2Obj["mtof5"] = opcn2Data.bin5_mtof;
    opcn2Obj["mtof7"] = opcn2Data.bin7_mtof;
    JsonArray &binCntArray = opcn2Obj.createNestedArray("cnt");
    for (int i=0; i<OPCN2Data::NumHistogramBins; i++)
    {
        binCntArray.add(opcn2Data.binCount[i]);
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


    // Write json object to open log
    rootObj.printTo(*serialPtr_);
    *serialPtr_ << endl;


    // Display the data
    
            SPI.beginTransaction(constants::DisplaySPISettings);            
            display.clearDisplay();
            display.setCursor(0,0);
            display.println("Gas Sensor (ppb)");

            for (int i=0; i<NUM_GAS_TYPE; i++)
            {
                if (GasTypeArray[i] == GAS_TYPE_NONE)
                {
                    continue;
                }

                int numType = 0;
                float cumValue = 0.0;

                for (auto &sensor : gasSensors)
                {
                    if (sensor.gasType() == GasTypeArray[i])
                    {
                        numType++;
                        cumValue += sensor.ppbLowPass();
                    }
                }
                if (numType > 0)
                {
                    int avgValue = cumValue/int(numType);
                    display.print("  ");
                    display.print(GasTypeToGasName[i]);
                    for (int j=0; j<8-GasTypeToGasName[i].length(); j++)
                    {
                        display.print(" "); 
                    }
                    display.println(avgValue);
                }
            } 

            display.setCursor(0,32);
            display.println("Particle counter");
            display.print("  PM1      ");
            display.println(opcn2Data.PM1);
            display.print("  PM2.5    ");
            display.println(opcn2Data.PM2_5);
            display.print("  PM10     ");
            display.println(opcn2Data.PM10);
            display.display();
            SPI.endTransaction();


    //DEV
    //-------------------------------------------------
    //rootObj.printTo(Serial);
    //rootObj.prettyPrintTo(Serial);
    //Serial << endl;
    
    rootObj.printTo(Serial2);
    Serial2 << endl;
    
    Serial << "cnt = " << count_ << ": ";
    Serial << opcn2Data.PM1  << ", ";  
    Serial << opcn2Data.PM2_5 << ", "; 
    Serial << opcn2Data.PM10 << endl;
    // -------------------------------------------------

}



