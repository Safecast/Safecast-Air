#ifndef OPEN_LOG_H
#define OPEN_LOG_H
#include <Arduino.h>
#include "openlog_param.h"

class Openlog
{
    public:

        static const int FilenameMaxLen;

        Openlog(OpenlogParam param);
        void initialize();

        bool gotoCommandMode();

        int  getFileSize(char filename[]);
        int  getFileSize(String filename);

        String readFile(char filename[]);
        String readFile(String filename);

        bool openNewFile(char filename);
        bool openNewFile(String filename);

        HardwareSerial *getSerialPtr();

        template<typename T> void print(T value)
        {
            param_.serialPtr -> print(value);
        };

        template<typename T> void println(T value)
        {
            param_.serialPtr -> println(value);
        };

    protected:  


        OpenlogParam param_;
        String getResponse(bool &ok);

};

#endif
