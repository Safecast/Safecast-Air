#include "opcn2_data.h"

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
    haveTemperature = false;
    pressure = 0;
    havePressure = false;
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
    uint32_t tempPressVal = 0;
    tempPressVal |= spiData[41];
    tempPressVal |= spiData[42] << 8;
    tempPressVal |= spiData[43] << 16;
    tempPressVal |= spiData[44] << 24;
    if (tempPressVal < 1000)
    {
        temperature = tempPressVal;
        pressure = 0;
        haveTemperature = true;
        havePressure = false;
    }
    else
    {
        pressure = tempPressVal;
        temperature = 0;
        haveTemperature = false;
        havePressure = true;
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

    // Add temperature or pressure
    if (haveTemperature)
    {
        snprintf(scratch,ScratchArraySize,"temperature = %d\n",temperature);
        dataStr += String(scratch);
    }
    if (havePressure)
    {
        snprintf(scratch,ScratchArraySize,"pressure = %d\n",pressure);
        dataStr += String(scratch);
    }

    // Add sample period
    snprintf(scratch,ScratchArraySize,"samplePeriod = %f\n",samplePeriod);
    dataStr += String(scratch);

    // Add checksum
    snprintf(scratch,ScratchArraySize,"checksum = %d\n",checksum);

    // Add PM1
    snprintf(scratch,ScratchArraySize,"PM1 = %f\n",PM1);
    dataStr += String(scratch);

    // Add PM2_5
    snprintf(scratch,ScratchArraySize,"PM2_5 = %f\n",PM2_5);
    dataStr += String(scratch);

    // Add PM_10
    snprintf(scratch,ScratchArraySize,"PM10 = %f\n",PM10);
    dataStr += String(scratch);

    return dataStr;

}
