#include "opcn2.h"
#include <Streaming.h>
#include "constants.h"
#include "fixed_vector.h"

OPCN2::OPCN2(OPCN2Param param)
{
    param_ = param;
};

void OPCN2::initialize()
{
    delay(StartupDelayMs); // Startup delay ~2s seems to be required
    pinMode(MOSI,OUTPUT);
    pinMode(SCK,OUTPUT);
    pinMode(MISO,INPUT);
    pinMode(param_.spiCsPin,OUTPUT);
    digitalWrite(param_.spiCsPin, HIGH);
    spiSettings_ = SPISettings(param_.spiClock, param_.spiBitOrder, param_.spiDataMode);
    SPI.begin();
    delay(StartupDelayMs); // Startup delay ~1s seems to be required
}

bool OPCN2::checkStatus()
{
    bool ok = false;
    SPI.beginTransaction(spiSettings_);
    digitalWrite(param_.spiCsPin,LOW);
    uint8_t rsp = SPI.transfer(CmdCheckStatus); // send cmd byte
    digitalWrite(param_.spiCsPin,HIGH);
    SPI.endTransaction();
    delay(SPICmdDelayMs);
    if (rsp == 0xf3)
    {
        ok = true;
    }
    return ok;
}


void OPCN2::setFanAndLaserOn(bool *ok)
{
    SPI.beginTransaction(spiSettings_);
    digitalWrite(param_.spiCsPin,LOW);
    uint8_t rsp0 = SPI.transfer(CmdLaserFanOnOff); // send cmd byte
    delay(SPICmdDelayMs);
    uint8_t rsp1 = SPI.transfer(0x00); // set laser and fan on
    digitalWrite(param_.spiCsPin,HIGH);
    SPI.endTransaction();
    delay(SPICmdDelayMs);
    if (ok != nullptr)
    {
        if ((rsp0 == 0xf3) && (rsp1 == 0x03))
        {
            *ok = true;
        }
        else
        {
            *ok = false;
        }
    }
}


void OPCN2::setFanAndLaserOff(bool *ok)
{
    SPI.beginTransaction(spiSettings_);
    digitalWrite(param_.spiCsPin,LOW);
    uint8_t rsp0 = SPI.transfer(CmdLaserFanOnOff); // send cmd byte
    delay(SPICmdDelayMs);
    uint8_t rsp1 = SPI.transfer(0x01); // set laser and fan off
    digitalWrite(param_.spiCsPin,HIGH);
    SPI.endTransaction();
    delay(SPICmdDelayMs);
    if (ok != nullptr)
    {
        if ((rsp0 == 0xf3) && (rsp1 == 0x03))
        {
            *ok = true;
        }
        else
        {
            *ok = false;
        }
    }
}


String OPCN2::getInfoString(bool *ok)
{
    uint8_t rsp[InfoMessageSize];

    SPI.beginTransaction(spiSettings_);
    digitalWrite(param_.spiCsPin,LOW);
    for (int i=0; i<InfoMessageSize; i++)
    {
        rsp[i] = SPI.transfer(CmdReadInfoString);
        if (i==0)
        {
            delay(SPICmdDelayMs);
        }
        else
        {
            delayMicroseconds(SPIValDelayUs);
        }
    }
    digitalWrite(param_.spiCsPin,HIGH);
    SPI.endTransaction();
    delay(SPICmdDelayMs);

    char infoChar[InfoStringLength+1];
    for (int i=0; i<InfoStringLength;i++)
    {
        infoChar[i] = char(rsp[i+2]);
    }
    infoChar[InfoStringLength] = '\0';
    String infoString(infoChar);

    if (ok != nullptr)
    {
        if (rsp[0] == 0xf3)
        {
            *ok = true;
        }
        else
        {
            *ok = false;
        }
    }
    return infoString;
}

OPCN2Data OPCN2::getHistogramData(bool *ok)
{
    uint8_t rsp[HistogramMessageSize];

    SPI.beginTransaction(spiSettings_);
    digitalWrite(param_.spiCsPin,LOW);
    for (int i=0; i<HistogramMessageSize; i++)
    {
        rsp[i] = SPI.transfer(CmdReadHistogramData);
        if (i==0)
        {
            delay(SPICmdDelayMs);
        }
        else
        {
            delayMicroseconds(SPIValDelayUs);
        }
        
    }
    digitalWrite(param_.spiCsPin,HIGH);
    SPI.endTransaction();
    delay(SPICmdDelayMs);

    return OPCN2Data(rsp);
}

//OPCN2 ParticleCounterOPCN2 = OPCN2(constants::DefaultOPCN2Param);
//OPCN2 ParticleCounterOPCN2 = OPCN2(constants::DefaultOPCN2Param);
OPCN2 ParticleCounter1 = OPCN2(constants::DefaultOPCN2Param);
OPCN2 ParticleCounter2 = OPCN2(constants::DefaultOPCN2Param2);
