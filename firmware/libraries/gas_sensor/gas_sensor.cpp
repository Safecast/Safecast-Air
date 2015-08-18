#include "gas_sensor.h"
#include <Streaming.h>
#include <cstdint>

const int   AinMaxInt   = UINT16_MAX;
const float MilliVoltPerVolt = 1000.0;
const float MilliSecPerSec = 1.0e-3;

const GasSensorParam UndefinedSensorParam = 
    {
        GAS_TYPE_NONE,   // gasType 
        0,               // serialNumber
        0,               // header
        0,               // position
        A0,              // workingAinPin 
        A1,              // auxillaryAinPin
        0.0,             // workingZero
        0.0,             // auxillaryZero
        1.0,             // sensitivity
        1.2,             // ainVRef
        1.0,             // ainScaleFact
        1.0,             // powerScaleFact
        1.0,             // lowPassCutoffFreq
        1,               // lowPassOrder
        false            // active
    };


// GasSensorDev public methods
// ------------------------------------------------------------------------

GasSensorDev::GasSensorDev() 
{
    setParam(UndefinedSensorParam);
    initialize();
} 


GasSensorDev::GasSensorDev(GasSensorParam param)
{
    setParam(param);
    initialize();
}


void GasSensorDev::sample(unsigned long dt)
{
    workingInt_   = analogRead(param_.workingAinPin);
    working_ = (param_.ainVRef*float(workingInt_)/float(AinMaxInt))*param_.ainScaleFact;
    workingZeroed_  = working_ - param_.workingZero/param_.powerScaleFact;

    auxillaryInt_ = analogRead(param_.auxillaryAinPin);
    auxillary_ = (param_.ainVRef*float(auxillaryInt_)/float(AinMaxInt))*param_.ainScaleFact;
    auxillaryZeroed_ = auxillary_ - param_.auxillaryZero/param_.powerScaleFact;

    ppb_ = MilliVoltPerVolt*param_.powerScaleFact*(workingZeroed_ - auxillaryZeroed_)/param_.sensitivity;
    //ppb_ = MilliVoltPerVolt*param_.powerScaleFact*workingZeroed_/param_.sensitivity;
    ppbLowPassFilter_.update(ppb_, MilliSecPerSec*dt);
}


GasSensorParam GasSensorDev::param() const
{
    return param_;
}


void GasSensorDev::setParam(GasSensorParam param)
{
    param_ = param;
    ppbLowPassFilter_ = filter::LowPass(param_.lowPassCutoffFreq, param_.lowPassOrder, 0.0);
}


float GasSensorDev::ppb() const
{
    return ppb_;
}


float GasSensorDev::ppbLowPass() const
{
    return ppbLowPassFilter_.value();
}


float GasSensorDev::ppm() const
{
    return (1.0e-3)*ppb_;
}


float GasSensorDev::ppmLowPass() const
{
    return (1.0e-3)*ppbLowPassFilter_.value();
}


int GasSensorDev::workingInt() const
{
    return workingInt_;
}


int GasSensorDev::auxillaryInt() const
{
    return auxillaryInt_;
}


float GasSensorDev::working() const
{
    return working_;
}


float GasSensorDev::auxillary() const
{
    return auxillary_;
}


float GasSensorDev::workingZeroed() const
{
    return workingZeroed_;
}


float GasSensorDev::auxillaryZeroed() const
    {
    return auxillaryZeroed_;
}


String GasSensorDev::gasName() const
{
    if (param_.gasType < NUM_GAS_TYPE)
    {
        return GasTypeToGasName[param_.gasType]; 
    }
    else
    {
        return GasTypeToGasName[GAS_TYPE_NONE];
    }
}


GasType GasSensorDev::gasType() const
{
    return param_.gasType;
}


bool GasSensorDev::isActive() const
{
    return param_.active;
}


void GasSensorDev::initialize()
{
    workingInt_ = 0;
    auxillaryInt_ = 0;
    working_ = 0.0;
    auxillary_ = 0.0;
    workingZeroed_ = 0.0;
    auxillaryZeroed_ = 0.0;
    ppb_ = 0.0;
    ppbLowPassFilter_.reset();
}



