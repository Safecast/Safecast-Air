#ifndef OPCN2_IDS_H
#define OPCN2_IDS_H
#include "opcn2_data.h"


struct OPCN2Ids
{
    static const int NumDataIds = 9;
    static const int NumHistogramIds = OPCN2Data::NumHistogramBins;
    static const int IndexHistogramIds = 9;

    unsigned int pm1;
    unsigned int pm2_5;
    unsigned int pm10;
    unsigned int rate;
    unsigned int dt;
    unsigned int mtof1;
    unsigned int mtof3;
    unsigned int mtof5;
    unsigned int mtof7;
    unsigned int cnt[NumHistogramIds];
};

#endif
