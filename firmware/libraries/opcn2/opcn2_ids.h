#ifndef OPCN2_IDS_H
#define OPCN2_IDS_H
#include "opcn2_data.h"

struct OPCN2Ids
{
    unsigned int pm1;
    unsigned int pm2_5;
    unsigned int pm10;
    unsigned int rate;
    unsigned int dt;
    unsigned int mtof1;
    unsigned int mtof3;
    unsigned int mtof5;
    unsigned int mtof7;
    unsigned int cnt[OPCN2Data::NumHistogramBins];
};

#endif
