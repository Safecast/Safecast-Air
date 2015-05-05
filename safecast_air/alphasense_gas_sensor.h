#ifndef ALPHASENSE_GAS_SENSOR_H
#define ALPHASENSE_GAS_SENSOR_H

namespace alphasense 
{

    enum GasType 
    {
        GAS_TYPE_NONE, 
        GAS_TYPE_NO2, 
        GAS_TYPE_NO, 
        GAS_TYPE_O3, 
        GAS_TYPE_CO, 
        GAS_TYPE_H2S, 
        GAS_TYPE_SO2
    };

    struct GasSensorParam
    {
        GasType gas_type;
        int wrk_ain;
        int aux_ain;
        float wrk_zero;
        float aux_zero;
        float sensitivity;
        float ain_scale;
        float pwr_scale;
        float lowpass_fc;
        int temp_sensor;
    };


    class GasSensor
    {
        public:
    
            GasSensor();
            GasSensor(GasSensorParam param);

            void setParam(GasSensorParam param);
            void sample(float dt);

            float getPPB();
            float getPPBFilt();

            int getWrkInt();
            int getAuxInt();

            float getWrkRaw();
            float getAuxRaw();
            
            float getWrkZeroed();
            float getAuxZeroed();

        protected:

            GasSensorParam param_;

            int wrk_int_;
            int aux_int_;

            float wrk_raw_;
            float aux_raw_;

            float wrk_zeroed_;
            float aux_zeroed_;

            float ppb_raw_;
            float ppb_filt_;

            void initValues();
    };


    void analogInputSetup();

}

#endif
