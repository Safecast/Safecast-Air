#include "opcn2_data.h"
#include "Streaming.h"

const int SCRATCH_SIZE = 50;

OPCN2Data::OPCN2Data()
{
    for (int i=0; i<NumHistogramBins; i++)
    {
        binCount[i] = 0;
    }
    bin1_mtof = 0;
    bin3_mtof = 0;
    bin5_mtof = 0;
    bin7_mtof = 0;
    flowRate = 0.0;
    temperature = 0;
    pressure = 0;
    samplePeriod = 0.0;
    checksum = 0;
    PM1 = 0.0;
    PM2_5 = 0.0;
    PM10 = 0.0;

}

OPCN2Data::OPCN2Data(uint8_t spiData[])
{
    fromSPIData(spiData);
}


void OPCN2Data::fromSPIData(uint8_t spiData[])
{
    uint8_t pos = 0; 

    // Get Bin counts
    for (int i=0; i<NumHistogramBins; i++)
    {
        binCount[i] = 0;
        for (int j=0; j<2; j++)
        {
            pos++;
            binCount[i] |=  (uint16_t(spiData[pos]) << 8*j);
        }
    }

    // Get mtof data
    bin1_mtof = spiData[33];
    bin3_mtof = spiData[34];
    bin5_mtof = spiData[35];
    bin7_mtof = spiData[36];

    // Get flow rate
    flowRate = *(float *) &spiData[37];

    // Get Temperature or pressure (alternating)
    uint32_t tempPressVal = *(uint32_t *) &spiData[38];
    if (tempPressVal < 1000)
    {
        temperature = tempPressVal;
        pressure = 0;
    }
    else
    {
        pressure = tempPressVal;
        temperature = 0;
    }

    // Get sampling period
    samplePeriod = *(float *) &spiData[45]; 

    // Get checksom
    checksum  = uint16_t(spiData[49]);
    checksum |= uint16_t(spiData[50]) << 8; 

    // Get PM  values
    PM1   = *(float *) &spiData[51];
    PM2_5 = *(float *) &spiData[55];
    PM10  = *(float *) &spiData[59];

}

String OPCN2Data::toString()
{
    String dataStr("");
    char scratch[ScratchArraySize]; 

    // Add Histogram data
    for (int i=0; i<NumHistogramBins; i++)
    {
        snprintf(scratch,ScratchArraySize,"bin[%d] = %d\n", i, binCount[i]);
        dataStr += String(scratch);
    }
    
    // Add MTOF data
    snprintf(scratch,ScratchArraySize,"bin1_mtof = %d\n", bin1_mtof);
    dataStr += String(scratch);

    snprintf(scratch,ScratchArraySize,"bin3_mtof = %d\n", bin3_mtof);
    dataStr += String(scratch);

    snprintf(scratch,ScratchArraySize,"bin5_mtof = %d\n", bin5_mtof);
    dataStr += String(scratch);

    snprintf(scratch,ScratchArraySize,"bin7_mtof = %d\n", bin7_mtof);
    dataStr += String(scratch);

    // Add flow rate
    snprintf(scratch,ScratchArraySize,"flowRate = %f\n", flowRate);
    dataStr += String(scratch);

    // Add PM1
    snprintf(scratch,ScratchArraySize,"PM1 = %f\n",PM1);
    dataStr += String(scratch);

    // Add PM2_5
    snprintf(scratch,ScratchArraySize,"PM2_5 = %f\n",PM2_5);
    dataStr += String(scratch);

    // Add PM_10
    snprintf(scratch,ScratchArraySize,"PM10 = %f\n",PM10);
    dataStr += String(scratch);

    //float flowRate;
    //uint32_t temperature;
    //uint32_t pressure;
    //float  samplePeriod;
    //uint16_t checksum;
    //float PM1;
    //float PM2_5;
    //float PM10;

    return dataStr;

}
