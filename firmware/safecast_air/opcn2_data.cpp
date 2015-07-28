#include "opcn2_data.h"
#include "Streaming.h"

OPCN2Data::OPCN2Data()
{
    for (int i=0; i<NUM_HISTOGRAM_BINS; i++)
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
    samplingPeriod = 0;
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
    for (int i=0; i<NUM_HISTOGRAM_BINS; i++)
    {
        binCount[i] = 0;
        for (int j=0; j<2; j++)
        {
            pos++;
            binCount[i] += (spiData[pos] << j);
        }
    }

    // Get mtof data
    pos++;
    bin1_mtof = spiData[pos];

    pos++;
    bin3_mtof = spiData[pos];

    pos++;
    bin5_mtof = spiData[pos];

    pos++;
    bin7_mtof = spiData[pos];

    // Get flow rate
    uint32_t flowRateUint32  = 0;
    for (int i=0; i<4; i++)
    {
        pos++;
        flowRateUint32 += (spiData[pos] << i);
    }
    flowRate = float(flowRateUint32);

    // Get Temperature or pressure (alternating)
    uint32_t tempPresUint32 = 0;
    for (int i=0; i<4; i++)
    {
        pos++;
        tempPresUint32 += (spiData[pos] << i);
    }
    if (tempPresUint32 < 1000)
    {
        temperature = tempPresUint32;
        pressure = 0;
    }
    else
    {
        pressure = tempPresUint32;
        temperature = 0;
    }

    // Get sampling period

}

