#include "i2c_t3.h"
#include "Adafruit_SHT31.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup()
{
    Serial.begin(9600);
    Serial.println("SHT31 test");
    if (!sht31.begin(0x44)) 
    {   
        while (1) 
        {
            Serial.println("Couldn't find SHT31");
            delay(1);
        }
    }
}


void loop()
{
    Serial.print("getting values ... ");
    float t = sht31.readTemperature();
    float h = sht31.readHumidity();
    Serial.println("done");

    if (!isnan(t)) 
    {  
        Serial.print("Temp *C = "); Serial.println(t);
    } 
    else 
    { 
        Serial.println("Failed to read temperature");
    }

    if (!isnan(h)) 
    {  
        Serial.print("Hum. % = "); Serial.println(h);
    } 
    else 
    { 
        Serial.println("Failed to read humidity");
    }
    Serial.println();

    delay(100);

}
