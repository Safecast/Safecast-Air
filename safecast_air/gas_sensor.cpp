#include "gas_sensor.h"
#include "constants.h"
#include "Arduino.h"
#include "Streaming.h"

const GasSensorParam UndefinedSensorParam = {GAS_TYPE_NONE, 0, 0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0};
const int   AinMaxInt   = 32767;
const float AinRefScale = 1.2;

// GasSensorDev public methods
// ------------------------------------------------------------------------

GasSensorDev::GasSensorDev() 
{
    setParam(UndefinedSensorParam);
    initializeValues();
} 


GasSensorDev::GasSensorDev(GasSensorParam param)
{
    setParam(param);
    initializeValues();
}

void GasSensorDev::sample(float dt)
{
    workingInt_ = analogRead(param_.workingAinPin);
    working_ = (AinRefScale*float(workingInt_)/float(AinMaxInt))*param_.ainScaleFact;
    workingZeroed_  = working_ - param_.workingZero/param_.powerScaleFact;

    auxillaryInt_ = analogRead(param_.auxillaryAinPin);
    auxillary_ = (AinRefScale*float(auxillaryInt_)/float(AinMaxInt))*param_.ainScaleFact;
    auxillaryZeroed_ = auxillary_ - param_.auxillaryZero/param_.powerScaleFact;

    ppb_ = 1000.0*param_.powerScaleFact*(workingZeroed_ - auxillaryZeroed_)/param_.sensitivity;
    ppbLowPassFilter_.update(ppb_, dt);
}


GasSensorParam GasSensorDev::param()
{
    return param_;
}

void GasSensorDev::setParam(GasSensorParam param)
{
    param_ = param;
    ppbLowPassFilter_ = filter::LowPass(param_.lowPassCutoffFreq, param_.lowPassOrder, 0.0);
}

float GasSensorDev::ppb()
{
    return ppb_;
}


float GasSensorDev::ppbLowPass()
{
    return ppbLowPassFilter_.value();
}


float GasSensorDev::ppm()
{
    return (1.0e-3)*ppb_;
}

float GasSensorDev::ppmLowPass()
{
    return (1.0e-3)*ppbLowPassFilter_.value();
}

int GasSensorDev::workingInt()
{
    return workingInt_;
}


int GasSensorDev::auxillaryInt()
{
    return auxillaryInt_;
}


float GasSensorDev::working()
{
    return working_;
}


float GasSensorDev::auxillary()
{
    return auxillary_;
}


float GasSensorDev::workingZeroed()
{
    return workingZeroed_;
}


float GasSensorDev::auxillaryZeroed()
    {
    return auxillaryZeroed_;
}


// GasSensorDev protected methods
// ------------------------------------------------------------------------

void GasSensorDev::initializeValues()
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


// GasSensorDevVector public methods
// ----------------------------------------------------------------------------
void GasSensorDevVector::initialize()
{
    setupAnalogInput();
    for (int i=0; i<size(); i++)
    {
        item_[i] = GasSensorDev(constants::DefaultGasSensorParam[i]);
    }
}

void GasSensorDevVector::setupAnalogInput()
{
    analogReadRes(constants::GasSensorAinResolution);
    analogReadAveraging(constants::GasSensorAinAveraging);
    analogReference(INTERNAL);
}


GasSensorDevVector GasSensors;



