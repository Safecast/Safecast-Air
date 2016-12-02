#include "openlog.h"
#include <EEPROM.h>

const int Openlog::FilenameMaxLen = 13;
const int Openlog::LogCountMaxVal = 65534;

// Openlog public methods
// ----------------------------------------------------------------------------

Openlog::Openlog(OpenlogParam param)
{
    param_ = param;
}


void Openlog::initialize()
{
    param_.serialPtr -> begin(param_.baudRate);
    pinMode(param_.resetPin, OUTPUT);
    reset();
    waitForPrompt();
}


void Openlog::reset()
{
    digitalWrite(param_.resetPin, LOW);
    delay(200);
    digitalWrite(param_.resetPin, HIGH);
}


void Openlog::waitForPrompt()
{
    while (true)
    {
        if (param_.serialPtr -> available())
        {
            if ((param_.serialPtr -> read()) == '>')
            {
                break;
            }
        }
    }
}


void Openlog::flush()
{
    param_.serialPtr -> flush();
    while (param_.serialPtr -> available())
    {
        param_.serialPtr -> read();
        delay(2);
    }
}


int  Openlog::getFileSize(char filename[])
{
    if (!gotoCommandMode())
    {
        return -1;
    }

    bool ok;
    int fileSize = -1;
    param_.serialPtr -> print("size ");
    param_.serialPtr -> print(filename);
    param_.serialPtr -> write('\r');
    String response = getResponse(ok);
    if (ok)
    {
        fileSize = response.toInt();
    }
    return fileSize;
}


int  Openlog::getFileSize(String filename)
{
    char buf[FilenameMaxLen];
    filename.toCharArray(buf,FilenameMaxLen);
    return getFileSize(buf);
}


String Openlog::readFile(char filename[])
{
    if (!gotoCommandMode())
    {
        return String("");
    }

    bool ok;
    param_.serialPtr -> print("read ");
    param_.serialPtr -> print(filename);
    param_.serialPtr -> write('\r');
    String contents = getResponse(ok);
    return contents;
}


String Openlog::readFile(String filename)
{
    char buf[FilenameMaxLen];
    filename.toCharArray(buf,FilenameMaxLen);
    return readFile(buf);
}


bool Openlog::openNewFile(char filename[])
{
    if (!gotoCommandMode())
    {
        return false;
    }

    // Create new file
    param_.serialPtr -> print("new ");
    param_.serialPtr -> print(filename);
    param_.serialPtr -> write('\r');
    bool done = false;
    bool ok = true; 
    for (unsigned long i=0; ((i<param_.timeout)&&(!done)); i++)
    {
        while(param_.serialPtr -> available())
        {
            char val = param_.serialPtr -> read();
            //Serial.println(uint8_t(val));
            
            if (val == '>')
            {
                done = true;
            }
            if (val == '!')
            {
                ok = false;
                done = true;
            }
        }
        delay(1);
    }
    if (!done)
    {
        ok = false;
        return ok;
    }

    // Open file for appending
    param_.serialPtr -> print("append ");
    param_.serialPtr -> print(filename);
    param_.serialPtr -> write('\r');
    done = false;
    ok = true; 
    for (unsigned long i=0; ((i<param_.timeout)&&(!done)); i++)
    {
        while(param_.serialPtr -> available())
        {
            char val = param_.serialPtr -> read();
            //Serial.println(uint8_t(val));
            
            if (val == '<')
            {
                done = true;
            }
            if (val == '!')
            {
                ok = false;
                done = true;
            }
        }
        delay(1);
    }
    if (!done)
    {
        ok = false;
    }
    return ok;
}


bool Openlog::openNewFile(String filename)
{
    char buf[FilenameMaxLen];
    filename.toCharArray(buf,FilenameMaxLen);
    return openNewFile(buf);
}


bool Openlog::openNewLogFile()
{
    uint16_t logCount = eepromGetLogCount();
    char logname[FilenameMaxLen];
    snprintf(logname, FilenameMaxLen,"LOG%05d.txt", logCount);
    bool rval = openNewFile(logname);
    if (rval)
    {
        eepromIncrLogCount();
    }
    return rval;
}


bool Openlog::gotoCommandMode()
{
    bool rval = false;

    param_.serialPtr -> write(26);
    param_.serialPtr -> write(26);
    param_.serialPtr -> write(26);
    param_.serialPtr -> write('\r');
    for (unsigned long i=0; i<param_.timeout; i++)
    {
        if (param_.serialPtr -> available())
        {
            if (param_.serialPtr -> read()=='>')
            {
                rval = true;
                break;
            }
        }
        delay(1);
    }

    return rval;
}


void Openlog::eepromIncrLogCount()
{
    uint16_t logCount = eepromGetLogCount();
    eepromSetLogCount(logCount+1);
}


void Openlog::eepromResetLogCount()
{
    EEPROM.put(param_.eepromAddrLogCnt,0);
}


uint16_t Openlog::eepromGetLogCount()
{
    uint16_t value;
    EEPROM.get(param_.eepromAddrLogCnt,value);
    if (value > LogCountMaxVal)
    {
        value = 0;
    }
    return value;
}


void Openlog::eepromSetLogCount(uint16_t value)
{
    EEPROM.put(param_.eepromAddrLogCnt,value);
}



HardwareSerial* Openlog::getSerialPtr()
{
    return param_.serialPtr;
}


// Openlog protected methods
// ----------------------------------------------------------------------------
String Openlog::getResponse(bool &ok)
{
    String response = "";
    bool done = false;
    ok = true;

    for (unsigned long i=0; ((i<param_.timeout)||!done); i++)
    {
        while (param_.serialPtr -> available()>0)
        {
            char val = param_.serialPtr -> read();
            if ((val == '\r') || (val == '\n'))
            {
                continue;
            }
            if (val == '>')
            {   
                done = true;
                break;
            }
            if (val == '!')
            {
                done = true;
                ok = false;
                break;
            }
            response += val;
        }
        delay(1);
    }
    if (!ok)
    {
        response = "";
    }
    return response;
}
