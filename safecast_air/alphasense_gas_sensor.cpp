#include "alphasense_gas_sensor.h"
#include "constants.h"
#include "Arduino.h"

#include "Streaming.h"

namespace alphasense
{
    const GasSensorParam UndefinedSensorParam = {GAS_TYPE_NONE, 0, 0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0};
    const int   AinMaxInt   = 32767;
    const float AinRefScale = 1.2;

    // GasSensor public methods
    // ------------------------------------------------------------------------
    
    GasSensor::GasSensor() 
    {
        setParam(UndefinedSensorParam);
        initializeValues();
    } 


    GasSensor::GasSensor(GasSensorParam param)
    {
        setParam(param);
        initializeValues();
    }

    void GasSensor::sample(float dt)
    {
        workingInt_   = analogRead(param_.workingAinPin);
        auxillaryInt_ = analogRead(param_.auxillaryAinPin);

        working_   = (AinRefScale*float(workingInt_)/float(AinMaxInt))*param_.ainScaleFact;
        auxillary_ = (AinRefScale*float(auxillaryInt_)/float(AinMaxInt))*param_.ainScaleFact;

        workingZeroed_   = working_ - param_.workingZero/param_.powerScaleFact;
        auxillaryZeroed_ = auxillary_ - param_.auxillaryZero/param_.powerScaleFact;

        ppb_ = 1000.0*param_.powerScaleFact*(workingZeroed_ - auxillaryZeroed_)/param_.sensitivity;
        ppbLowPassFilter_.update(ppb_, dt);
    }


    GasSensorParam GasSensor::param()
    {
        return param_;
    }

    void GasSensor::setParam(GasSensorParam param)
    {
        param_ = param;
        ppbLowPassFilter_ = filter::LowPass(param_.lowPassCutoffFreq, param_.lowPassOrder, 0.0);
    }

    float GasSensor::ppb()
    {
        return ppb_;
    }


    float GasSensor::ppbLowPass()
    {
        return ppbLowPassFilter_.value();
    }


    float GasSensor::ppm()
    {
        return (1.0e-3)*ppb_;
    }

    float GasSensor::ppmLowPass()
    {
        return (1.0e-3)*ppbLowPassFilter_.value();
    }

    int GasSensor::workingInt()
    {
        return workingInt_;
    }


    int GasSensor::auxillaryInt()
    {
        return auxillaryInt_;
    }


    float GasSensor::working()
    {
        return working_;
    }


    float GasSensor::auxillary()
    {
        return auxillary_;
    }


    float GasSensor::workingZeroed()
    {
        return workingZeroed_;
    }


    float GasSensor::auxillaryZeroed()
        {
        return auxillaryZeroed_;
    }



    // GasSensor protected methods
    // ------------------------------------------------------------------------

    void GasSensor::initializeValues()
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


    // Utiliy functions
    void analogInputSetup()
    {
        analogReadRes(constants::GasSensorAinResolution);
        analogReadAveraging(constants::GasSensorAinAveraging);
        analogReference(INTERNAL);
    }

}
