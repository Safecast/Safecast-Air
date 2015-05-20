#include "lookup_table.h"
#include "utility.h"


LookupTable::LookupTable() 
{
    size = 0;
}

bool LookupTable::setTable(float _table[][2], unsigned int _size) 
{
    bool rtnVal = true;
    table = _table;
    size = _size;
    for (int i=1; i<size; i++) {
        if (table[i-1][0] > table[i][0]) {
            rtnVal = false;
        }
    }
    return rtnVal;
}


float LookupTable::getValue(float x) 
{
    int rtnVal=0;

    // handle x values outside of range - return nearest end point value.
    if (x <= table[0][0]) {
        rtnVal = table[0][1];
    }
    else if (x >= table[size-1][0]) {
        rtnVal = table[size-1][1];
    }
    else {
        // x value is inside table - interpolate
        for (int i=1; i<size; i++) {
            if ((x >= table[i-1][0]) && (x < table[i][0])) {
                rtnVal = mapFloat(x,table[i-1][0], table[i][0], table[i-1][1], table[i][1]);
                break;
            }
        }
    }
    return rtnVal;
}



