#include "opcn2.h"
#include <Streaming.h>
#include "constants.h"

OPCN2::OPCN2(OPCN2Param param)
{
    param_ = param;
};

void OPCN2::initialize()
{
    pinMode(MOSI,OUTPUT);
    pinMode(SCK,OUTPUT);
    pinMode(MISO,INPUT);
    pinMode(param_.spiCsPin,OUTPUT);
    digitalWrite(param_.spiCsPin, HIGH);
    spiSettings_ = SPISettings(param_.spiClock, param_.spiBitOrder, param_.spiDataMode);
    SPI.begin();
}

bool OPCN2::checkStatus()
{
    SPI.beginTransaction(spiSettings_);
    digitalWrite(param_.spiCsPin,LOW);
    delay(10);
    uint16_t cnt = 0;
    bool ok = false;
    while (cnt < 100)
    {
        uint8_t rsp = SPI.transfer(0xcf);
        if (rsp == 0xf3)
        {
            ok = true;
            break;
        }
        delay(50);
        cnt++;
    }
    digitalWrite(param_.spiCsPin,HIGH);
    SPI.endTransaction();
    return ok;
}

void OPCN2::setFanAndLaserOn()
{
    SPI.beginTransaction(spiSettings_);
    digitalWrite(param_.spiCsPin,LOW);
    delay(10);
    bool done = false;
    int cnt = 0;
    while (!done)
    {
        uint8_t rval0 = SPI.transfer(0x03);
        delay(10);
        uint8_t rval1 = SPI.transfer(0x00);
        if (rval1 == 0x03)
        {
            done = true;
        }
        Serial << "on: " << cnt << ", " << rval0 << ", "  << rval1  << endl;
        delay(10);
        cnt++;
    }
    Serial << endl;
    digitalWrite(param_.spiCsPin,HIGH);
    SPI.endTransaction();
}

void OPCN2::setFanAndLaserOff()
{
    SPI.beginTransaction(spiSettings_);
    digitalWrite(param_.spiCsPin,LOW);
    delay(10);
    bool done = false;
    int cnt = 0;
    while (!done)
    {
        uint8_t rval0 = SPI.transfer(0x03);
        delay(10);
        uint8_t rval1 = SPI.transfer(0x01);
        if (rval1 == 0x03)
        {
            done = true;
        }
        Serial << "off: " << cnt << ", " << rval0 << ", "  << rval1 << endl;
        delay(10);
        cnt++;
    }
    Serial << endl;
    digitalWrite(param_.spiCsPin,HIGH);
    SPI.endTransaction();
}

void OPCN2::getInfoString()
{
    SPI.beginTransaction(spiSettings_);
    digitalWrite(param_.spiCsPin,LOW);
    delay(10);
    for (int i=0; i<61; i++)
    {
        uint8_t rval;
        if (i==0)
        {
            rval = SPI.transfer(0x3f);
            delay(30);
            Serial << "i: " << i << ", " << _HEX(rval) << endl;
        }
        else
        {
            rval = SPI.transfer(0x3f);
            delayMicroseconds(10);
            Serial << "i: " << i << ", " << char(rval) << endl;
        }
    }

    digitalWrite(param_.spiCsPin,HIGH);
    SPI.endTransaction();
}

void OPCN2::getHistogram()
{
    SPI.beginTransaction(spiSettings_);
    digitalWrite(param_.spiCsPin,LOW);
    delay(10);
    for (int i=0; i<63; i++)
    {
        uint8_t rval;
        if (i==0)
        {
            rval = SPI.transfer(0x30);
            delay(30);
        }
        else
        {
            rval = SPI.transfer(0x30);
            delayMicroseconds(10);
        }
        Serial << "i: " << i << ", " << _HEX(rval) << endl;
    }

    digitalWrite(param_.spiCsPin,HIGH);
    SPI.endTransaction();

    Serial << endl;
}

OPCN2 ParticleCounterOPCN2 = OPCN2(constants::DefaultOPCN2Param);
