#include "openlog.h"

const int Openlog::FilenameMaxLen = 13;

// Openlog public methods
// ----------------------------------------------------------------------------
Openlog::Openlog(OpenlogParam param)
{
    param_ = param;
}


void Openlog::initialize()
{
    param_.serialPtr -> begin(param_.baudRate);
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
    param_.serialPtr -> print('\r');
    String contents = getResponse(ok);
    return contents;
}


String Openlog::readFile(String filename)
{
    char buf[FilenameMaxLen];
    filename.toCharArray(buf,FilenameMaxLen);
    return readFile(buf);
}


bool  Openlog::openNewFile(char filename)
{
    if (!gotoCommandMode())
    {
        return false;
    }

    param_.serialPtr -> print("new ");
    param_.serialPtr -> print(filename);
    param_.serialPtr -> print('\r');
    bool done = false;
    for (unsigned long i=0; ((i<param_.timeout)||(!done)); i++)
    {
        while(param_.serialPtr -> available())
        {
            if (param_.serialPtr -> read() == '<')
            {
                done = true;
            }
        }
    }
    return done;
}


bool Openlog::openNewFile(String filename)
{
    char buf[FilenameMaxLen];
    filename.toCharArray(buf,FilenameMaxLen);
    return openNewFile(buf);
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
