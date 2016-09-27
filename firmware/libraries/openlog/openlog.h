#ifndef OPEN_LOG_H
#define OPEN_LOG_H
#include <Arduino.h>
#include "openlog_param.h"

class Openlog
{
    public:

        static const int FilenameMaxLen;
        static const int LogCountMaxVal;

        Openlog(OpenlogParam param);
        void initialize();
        void reset();
        void waitForPrompt();
        void flush();

        bool gotoCommandMode();
        int  getFileSize(char filename[]);
        int  getFileSize(String filename);

        String readFile(char filename[]);
        String readFile(String filename);

        bool openNewFile(char filename[]);
        bool openNewFile(String filename);
        bool openNewLogFile(); // Automatic filenameing via eeprom log count

        void eepromIncrLogCount();
        void eepromResetLogCount();
        uint16_t eepromGetLogCount();
        void eepromSetLogCount(uint16_t value);

        HardwareSerial *getSerialPtr();

        template<typename T> void print(T value)
        {
            param_.serialPtr -> print(value);
        };


        void println()
        {
            param_.serialPtr -> println();
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
