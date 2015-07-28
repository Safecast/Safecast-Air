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
    delay(STARTUP_DELAY_MS); // Startup delay ~2s seems to be required
    pinMode(MOSI,OUTPUT);
    pinMode(SCK,OUTPUT);
    pinMode(MISO,INPUT);
    pinMode(param_.spiCsPin,OUTPUT);
    digitalWrite(param_.spiCsPin, HIGH);
    spiSettings_ = SPISettings(param_.spiClock, param_.spiBitOrder, param_.spiDataMode);
    SPI.begin();
    delay(STARTUP_DELAY_MS); // Startup delay ~1s seems to be required
}

bool OPCN2::checkStatus()
{
    bool ok = false;
    SPI.beginTransaction(spiSettings_);
    digitalWrite(param_.spiCsPin,LOW);
    uint8_t rsp = SPI.transfer(CMD_CHECK_STATUS); // send cmd byte
    digitalWrite(param_.spiCsPin,HIGH);
    SPI.endTransaction();
    delay(SPI_CMD_DELAY_MS);
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
    uint8_t rsp0 = SPI.transfer(CMD_LASER_FAN_ON_OFF); // send cmd byte
    delay(SPI_CMD_DELAY_MS);
    uint8_t rsp1 = SPI.transfer(0x00); // set laser and fan on
    digitalWrite(param_.spiCsPin,HIGH);
    SPI.endTransaction();
    delay(SPI_CMD_DELAY_MS);
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
    uint8_t rsp0 = SPI.transfer(CMD_LASER_FAN_ON_OFF); // send cmd byte
    delay(SPI_CMD_DELAY_MS);
    uint8_t rsp1 = SPI.transfer(0x01); // set laser and fan off
    digitalWrite(param_.spiCsPin,HIGH);
    SPI.endTransaction();
    delay(SPI_CMD_DELAY_MS);
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
    uint8_t rsp[INFO_MESSAGE_SIZE];

    SPI.beginTransaction(spiSettings_);
    digitalWrite(param_.spiCsPin,LOW);
    for (int i=0; i<INFO_MESSAGE_SIZE; i++)
    {
        rsp[i] = SPI.transfer(CMD_READ_INFO_STRING);
        if (i==0)
        {
            delay(SPI_CMD_DELAY_MS);
        }
        else
        {
            delayMicroseconds(SPI_VAL_DELAY_US);
        }
    }
    digitalWrite(param_.spiCsPin,HIGH);
    SPI.endTransaction();
    delay(SPI_CMD_DELAY_MS);

    char infoChar[INFO_STRING_LENGTH+1];
    for (int i=0; i<INFO_STRING_LENGTH;i++)
    {
        infoChar[i] = char(rsp[i+2]);
    }
    infoChar[INFO_STRING_LENGTH] = '\0';
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
    uint8_t rsp[HISTOGRAM_MESSAGE_SIZE];

    SPI.beginTransaction(spiSettings_);
    digitalWrite(param_.spiCsPin,LOW);
    for (int i=0; i<HISTOGRAM_MESSAGE_SIZE; i++)
    {
        rsp[i] = SPI.transfer(CMD_READ_HISTOGRAM_DATA);
        if (i==0)
        {
            delay(SPI_CMD_DELAY_MS);
        }
        else
        {
            delayMicroseconds(SPI_VAL_DELAY_US);
        }
        
    }
    digitalWrite(param_.spiCsPin,HIGH);
    SPI.endTransaction();
    delay(SPI_CMD_DELAY_MS);

    OPCN2Data data = OPCN2Data(rsp);
    for (int i=0; i<HISTOGRAM_MESSAGE_SIZE; i++)
    {
        Serial << "i: " << i << ", " << rsp[i] << endl;
    }
    return data;
}

OPCN2 ParticleCounterOPCN2 = OPCN2(constants::DefaultOPCN2Param);
