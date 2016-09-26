#include <ArduinoJson.h>
#include <EEPROM.h>
#include "openlog.h"

OpenlogParam openlogParam = {115200, 500, 0, &Serial3} ;
Openlog openlog(openlogParam);

void setup()
{
    Serial.begin(115200);
    openlog.initialize();
    delay(1000);

}

void loop()
{

    String filename = "SAFECAST.txt";

    int fileSize = openlog.getFileSize(filename);
    Serial.print("fileSize: ");
    Serial.println(fileSize);

    String fileContents = openlog.readFile(filename);
    Serial.print("json string = ");
    Serial.println(fileContents);

    StaticJsonBuffer<1000> jsonBuffer;
    JsonObject &jsonRoot  = jsonBuffer.parseObject((char *)(fileContents.c_str()));
    if (!jsonRoot.success())
    {
        Serial.println("parsing json failed");
    }
    else
    {
        Serial.println("parsin json success");
    }

    if (jsonRoot.containsKey("username"))
    {
        const char *username = jsonRoot["username"];
        Serial.print("username = ");
        Serial.println(username);
    }

    if (jsonRoot.containsKey("device_id"))
    {
        const char *device_id = jsonRoot["device_id"];
        Serial.print("device_id = ");
        Serial.println(device_id);
    }

    if (jsonRoot.containsKey("wifi_ssid"))
    {
        const char *wifi_ssid = jsonRoot["wifi_ssid"];
        Serial.print("wifi_ssid = ");
        Serial.println(wifi_ssid);
    }

    if (jsonRoot.containsKey("wifi_pass"))
    {
        const char *wifi_pass = jsonRoot["wifi_pass"];
        Serial.print("wifi_pass = ");
        Serial.println(wifi_pass);
    }

    if (jsonRoot.containsKey("opcn2_ids"))
    {
        JsonArray &opcn2Json = jsonRoot["opcn2_ids"];
        Serial.println("have opcn2_ids");
        int opcn2JsonLen = opcn2Json.size();
        Serial.print("opcn2JsonLen = ");
        Serial.println(opcn2JsonLen);
        for (int i=0; i<opcn2JsonLen-1; i++)
        {
            int value = opcn2Json[i];
            Serial.print("opcn2_ids[");
            Serial.print(i);
            Serial.print("] = ");
            Serial.println(value);
        }
        Serial.print("opcn2_ids[");
        Serial.print(opcn2JsonLen-1);
        Serial.print("] = [");
        JsonArray &binIdJson = opcn2Json[opcn2JsonLen-1];
        int  binIdJsonLen = binIdJson.size();
        for (int i=0; i<binIdJsonLen;i++)
        {
           int value = binIdJson[i];
           Serial.print(value);
           if (i<binIdJsonLen-1)
           {
               Serial.print(",");
           }
        }
        Serial.println("]");
    }

    Serial.println();
    Serial.println();
    Serial.println();

    delay(200);
}


