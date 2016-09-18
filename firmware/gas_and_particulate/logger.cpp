#include "logger.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GPS.h>
#include "constants.h"
#include "opcn2_ids.h"
#include "gas_sensor.h"
#include "tmp_sensor.h"

extern GasSensorDevVector<constants::NumGasSensor> gasSensors;
extern TmpSensorDevVector<constants::NumTmpSensor> tmpSensors;
extern GPSMonitor gpsMonitor;
extern OPCN2 particleCounter;
extern Adafruit_SSD1306 display;
extern MQ4_Methane methaneSensor;

Logger::Logger(LoggerParam param, HardwareSerial *serialPtr)
{
    param_ = param;
    serialPtr_ = serialPtr;
}


void Logger::initialize()
{
    serialPtr_ -> begin(param_.baudRate);
}

unsigned long Logger::period()
{
    return param_.period;
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
        writeDisplay();
        writeDataFlag_ = false;
    }
}

void Logger::writeConfiguration()
{
    // Not done ... 
    
    jsonBuffer_ = StaticJsonBuffer<JsonBufferSize>(); // Clear the buffer 
    JsonObject &rootObj = jsonBuffer_.createObject();
    rootObj["dev"] = constants::DeviceName;  // Change to something from loaded settings
    rootObj["id"] = constants::DeviceId;     // Change to something from loaded settings
    rootObj["msg"] = "cfg";

    // Add gas sensor metadata
    //JsonArray &gasArray = rootObj.createNestedArray("gas");
    //rootObj.printTo(*serialPtr_);
    //seialPtr_ -> println();
    // Dev
    // ----------------------------------------
    //rootObj.prettyPrintTo(Serial);
    //Serial.println();
    // ----------------------------------------

}


void Logger::writeData()
{
    opcn2Data_ = particleCounter.getHistogramData();
    OPCN2Ids opcn2Ids = particleCounter.getIds();

    bool gpsDataOk = false;
    if (gpsMonitor.haveData())
    {
        gpsData_ = gpsMonitor.getData(&gpsDataOk);
    }
    if (!gpsDataOk)
    {
        return;
    }

    String dateTimeString = gpsData_.getDateTimeString();
    String latitudeString = gpsData_.getLatitudeString();
    String longitudeString = gpsData_.getLongitudeString();

    // Create Json output data
    jsonBuffer_ = StaticJsonBuffer<JsonBufferSize>(); // Clear the buffer 
    JsonObject &rootObj = jsonBuffer_.createObject();
    rootObj["dev"] = constants::DeviceName;           // Change to something from loaded settings
    rootObj["id"] = constants::DeviceId;              // Change to something from loaded settings
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

    // Add UEXT array
    {
        JsonArray &uextArray = rootObj.createNestedArray("uext");
        JsonObject &sensorObj = uextArray.createNestedObject();
        sensorObj["type"] = methaneSensor.type().c_str();
        sensorObj["ain"] = methaneSensor.ain();
        sensorObj["ppm"] = methaneSensor.ppm();
        sensorObj["ppmFlt"] = methaneSensor.ppmLowPass();

        JsonObject &idsObj = sensorObj.createNestedObject("ids");
        MQ4_MethaneParam param = methaneSensor.getParam();
        idsObj["ain'"] = param.ids.ain;
        idsObj["ppm'"] = param.ids.ppm;
        idsObj["ppmFlt'"] = param.ids.ppmFlt;
    }

    // Add OPN2 sensor  ...
    // TO DO modifiy to handle multiple opcn2 devices.
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

    // Write json object to open log
    rootObj.printTo(*serialPtr_);
    serialPtr_ -> println();
    
#ifdef WITH_ROBS_PARAMS
    rootObj.printTo(Serial2);
    Serial2.println();
#endif

}


void Logger::writeDisplay() // Temporary hack - create separate display function at some point.
{
    // Display the data
#ifdef WITH_SEANS_PARAMS
    const int col2 = 50;
    const int col3 = 90;
    SPI.beginTransaction(constants::DisplaySPISettings);            
    display.clearDisplay();
    display.setCursor(0,0);
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
            display.print(GasTypeToGasName[i]);
            display.setCursor(col2,display.getCursorY());
            display.print(avgValue);
            display.setCursor(col3,display.getCursorY());
            display.println("ppb");
        }
    } 

    display.print("CH4");
    display.setCursor(col2,display.getCursorY());
    display.print(methaneSensor.ppmLowPass());
    display.setCursor(col3,display.getCursorY());
    display.println("ppm");

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
    display.display();
    SPI.endTransaction();

#endif
#ifdef WITH_ROBS_PARAMS
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
    display.println(opcn2Data_.PM1);
    display.print("  PM2.5    ");
    display.println(opcn2Data_.PM2_5);
    display.print("  PM10     ");
    display.println(opcn2Data_.PM10);
    display.display();
    SPI.endTransaction();
#endif

}
