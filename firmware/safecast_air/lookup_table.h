#ifndef LOOKUP_TABLE_H
#define LOOKUP_TABLE_H
#include "Arduino.h"

class LookupTable {
    public:
        LookupTable();
        float getValue(float x);
        bool setTable(float _table[][2], unsigned int _size);
    private:
        unsigned int size;
        float (*table)[2];

};

#endif
