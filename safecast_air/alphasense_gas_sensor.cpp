#include "alphasense_gas_sensor.h"
#include "constants.h"
#include "Arduino.h"

#include "Streaming.h"

namespace alphasense
{
    const GasSensorParam undefined_sensor_param = {GAS_TYPE_NONE, 0, 0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0};
    const int ain_max_int = 32767;
    const float ain_ref_scale = 1.2;

    // GasSensor public methods
    // ------------------------------------------------------------------------
    
    GasSensor::GasSensor() 
    {
        setParam(undefined_sensor_param);
        initValues();
    } 


    GasSensor::GasSensor(GasSensorParam param)
    {
        setParam(param);
        initValues();
    }


    void GasSensor::setParam(GasSensorParam param)
    {
        param_ = param;
    }


    void GasSensor::sample(float dt)
    {
        wrk_int_ = analogRead(param_.wrk_ain);
        aux_int_ = analogRead(param_.aux_ain);

        wrk_raw_ = (ain_ref_scale*float(wrk_int_)/float(ain_max_int))*param_.ain_scale*param_.pwr_scale;
        aux_raw_ = (ain_ref_scale*float(aux_int_)/float(ain_max_int))*param_.ain_scale*param_.pwr_scale;

        wrk_zeroed_ = wrk_raw_ - param_.wrk_zero;
        aux_zeroed_ = aux_raw_ - param_.aux_zero;

        ppb_raw_ = (wrk_zeroed_ - aux_zeroed_)/param_.sensitivity;

        // Compute lowpass filtered ppb - simple first order lowpass
        float rc = 1.0/(2.0*M_PI*param_.lowpass_fc);
        float alpha = dt/(rc + dt);
        ppb_filt_ = alpha*ppb_filt_ + (1.0-alpha)*ppb_raw_;
    }


    float GasSensor::getPPB()
    {
        return ppb_raw_;
    }


    float GasSensor::getPPBFilt()
    {
        return ppb_filt_;
    }


    int GasSensor::getWrkInt()
    {
        return wrk_int_;
    }


    int GasSensor::getAuxInt()
    {
        return aux_int_;
    }


    float GasSensor::getWrkRaw()
    {
        return wrk_raw_;
    }


    float GasSensor::getAuxRaw()
    {
        return aux_raw_;
    }


    float GasSensor::getWrkZeroed()
    {
        return wrk_zeroed_;
    }


    float GasSensor::getAuzZeroed()
    {
        return aux_zeroed_;
    }

    // GasSensor protected methods
    // ------------------------------------------------------------------------

    void GasSensor::initValues()
    {
        wrk_int_ = 0;
        aux_int_ = 0;
        
        wrk_raw_ = 0.0;
        aux_raw_ = 0.0;

        wrk_zeroed_ = 0.0;
        aux_zeroed_ = 0.0;

        ppb_raw_ = 0.0;
        ppb_filt_ = 0.0;
    }


    // Utiliy functions
    void analogInputSetup()
    {
        analogReadRes(constants::gas_sensor_ain_res);
        analogReadAveraging(constants::gas_sensor_ain_avg);
        analogReference(INTERNAL);
    }
}
