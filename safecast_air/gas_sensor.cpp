#include "gas_sensor.h"
#include "constants.h"
#include <Arduino.h>
#include <Streaming.h>
#include <TimerThree.h>

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


// GasSensorDevVector public methods
// ----------------------------------------------------------------------------
void GasSensorDevVector::initialize()
{
    setupAnalogInput();
    for (int i=0; i<size(); i++)
    {
        set(i,GasSensorDev(constants::DefaultGasSensorParam[i]));
    }

    Timer3.initialize();
    Timer3.setPeriod(getSampleDtUs());
    Timer3.stop();
    Timer3.disablePwm(25);
    Timer3.disablePwm(32);
    Timer3.attachInterrupt(GasSensorDevVector::onTimerOverflow);
}

void GasSensorDevVector::sample()
{
    for (auto &sensor : *this)
    {
        sensor.sample(constants::GasSensorSampleDt);
    }
}

void GasSensorDevVector::start()
{
    Serial << "hello" << endl;
    for (auto &sensor: *this)
    {
        sensor.initialize();
    }
    // Note, it seems that Time3's start method doesn't work on the teensy 3.1
    // However, we can use setPeriod to restart the timer.
    Timer3.setPeriod(getSampleDtUs()); 
}

void GasSensorDevVector::stop()
{
    Timer3.stop();
}

// GasSensorDevVector protected methods
// ----------------------------------------------------------------------------
void GasSensorDevVector::setupAnalogInput()
{
    analogReadRes(constants::GasSensorAinResolution);
    analogReadAveraging(constants::GasSensorAinAveraging);
    analogReference(INTERNAL);
}

unsigned long GasSensorDevVector::getSampleDtUs()
{
    return 1000ul*constants::GasSensorSampleDt;
}


void GasSensorDevVector::onTimerOverflow()
{
    GasSensors.sample();
}


GasSensorDevVector GasSensors;



