#include <ESP8266.h>
#include <ArduinoJson.h>

const int PwrPin = 31;

ESP8266 wifi; 

const String SSID("mortivilla");
const String Password("kawasaki650"); 
const String Hostname("192.168.1.107");
const uint32_t Hostport = 5001;
const int JsonBufferSize = 4000;
const int MaxFailCount = 10;
bool wifi_ok = false;

bool setup_wifi_device();
void toggle_wifi_power();


void setup()
{

    Serial.begin(115200);
    wifi_ok = setup_wifi_device();
    delay(1000);
}


void loop()
{
    static int cnt = 0;
    static int fail_cnt = 0;
    static int reset_cnt = 0;
    static StaticJsonBuffer<JsonBufferSize> jsonBuf;

    if (wifi_ok)
    {
        Serial.print("cnt: ");
        Serial.println(cnt);
        Serial.print("fail_cnt: ");
        Serial.println(fail_cnt);
        Serial.print("reset_cnt: ");
        Serial.println(reset_cnt);

        jsonBuf = StaticJsonBuffer<JsonBufferSize>(); // Clear the jsonCharBuffer 
        JsonObject &testObj = jsonBuf.createObject();
        testObj["value"] = "my_value";
        testObj["unit"] =  "my_unit";
        testObj["count"] = cnt;

        char jsonCharBuf[JsonBufferSize];
        testObj.printTo(jsonCharBuf,JsonBufferSize);
        String jsonCharBufLen = String(strlen(jsonCharBuf));

        String sendData;
        sendData += String("POST /jsontest HTTP/1.1\r\n");
        sendData += String("Connection: close\r\n");
        sendData += String("Host: ") + Hostname + String(":") + String(Hostport) + String("\r\n");
        sendData += String("User-Agent: Arduino\r\n");
        sendData += String("Content-Type: application/json\r\n");
        sendData += String("Content-Length: ") + String(jsonCharBufLen)+ String("\r\n\r\n");
        sendData += String(jsonCharBuf);
        sendData += String("\r\n");
        const char *sendBuf = sendData.c_str();

        Serial.print("connect to host: ");
        if (wifi.createTCP(Hostname, Hostport))
        {
            Serial.println("ok");

            wifi.send((const uint8_t *)sendBuf, strlen(sendBuf));

            uint8_t recvBuf[1024] = {0};
            uint32_t len = wifi.recv(recvBuf, sizeof(recvBuf), 2000);
            if (len > 0) 
            {
                Serial.print("Received:");
                for(uint32_t i = 0; i < len; i++) 
                {
                    Serial.print((char)recvBuf[i]);
                }
            }
            cnt++;
            delay(5000);
        } 
        else 
        {
            Serial.println("err");
            fail_cnt++;
        }
    }
    else
    {
        if (fail_cnt >= MaxFailCount)
        {
            wifi_ok = setup_wifi_device();
            delay(2000); 
            reset_cnt++;
            fail_cnt = 0;
        }
    }
    Serial.println();
}


bool setup_wifi_device()
{
    toggle_wifi_power();

    wifi = ESP8266(Serial2);

    Serial.print("set to station mode: ");
    if (wifi.setOprToStation()) 
    {
        Serial.println("ok");
    } 
    else 
    {
        Serial.println("err");
        return false;
    }
    Serial.print("FW Version: ");
    Serial.println(wifi.getVersion());

    Serial.print("join ap: ");
    if (wifi.joinAP(SSID, Password)) 
    {
        Serial.println("ok");
    } 
    else 
    {
        Serial.println("fail");
        return false;
    }
    Serial.print("ip: ");
    Serial.println(wifi.getLocalIP().c_str());

    Serial.print("disable mux: ");
    if (wifi.disableMUX()) 
    {
        Serial.print("ok");
    } 
    else 
    {
        Serial.print("err");
        return false;
    }
    Serial.println();
    Serial.println();
    return true;
}


void toggle_wifi_power()
{
    digitalWrite(PwrPin,HIGH);
    pinMode(PwrPin,OUTPUT);
    delay(1000);
    digitalWrite(PwrPin,LOW);
    delay(4000);
}
