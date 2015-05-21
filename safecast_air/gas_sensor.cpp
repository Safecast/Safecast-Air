#include "gas_sensor.h"
#include "constants.h"
#include <Streaming.h>

const GasSensorParam UndefinedSensorParam = {GAS_TYPE_NONE, 0, 0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0};
const int   AinMaxInt   = 32767;
const float AinRefScale = 1.2;
const float MilliVoltPerVolt = 1000.0;
const float MilliSecPerSec = 1.0e-3;

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
    workingInt_ = analogRead(param_.workingAinPin);
    working_ = (AinRefScale*float(workingInt_)/float(AinMaxInt))*param_.ainScaleFact;
    workingZeroed_  = working_ - param_.workingZero/param_.powerScaleFact;

    auxillaryInt_ = analogRead(param_.auxillaryAinPin);
    auxillary_ = (AinRefScale*float(auxillaryInt_)/float(AinMaxInt))*param_.ainScaleFact;
    auxillaryZeroed_ = auxillary_ - param_.auxillaryZero/param_.powerScaleFact;

    ppb_ = MilliVoltPerVolt*param_.powerScaleFact*(workingZeroed_ - auxillaryZeroed_)/param_.sensitivity;
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


String GasSensorDev::paramToString() const
{
    // To Do ...
    String paramString = "";
    return paramString;
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


// GasSensorDevVector protected methods
// ----------------------------------------------------------------------------
void GasSensorDevVector::onTimerOverflow()
{
    GasSensors.sample();
}

GasSensorDevVector GasSensors(constants::GasSensorSamplingParam, constants::DefaultGasSensorParam); 





