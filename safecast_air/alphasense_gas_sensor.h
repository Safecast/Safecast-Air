#ifndef ALPHASENSE_GAS_SENSOR_H
#define ALPHASENSE_GAS_SENSOR_H
#include "filter.h"

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
        GasType gasType;
        int workingAinPin;
        int auxillaryAinPin;
        float workingZero;
        float auxillaryZero;
        float sensitivity;
        float ainScaleFact;
        float powerScaleFact;
        float lowPassCutoffFreq;
        unsigned int lowPassOrder;
        int temperatureSensor;
    };


    class GasSensor
    {
        public:
    
            GasSensor();
            GasSensor(GasSensorParam param);

            GasSensorParam param();
            void setParam(GasSensorParam param);
            void sample(float dt);

            float ppb();
            float ppbLowPass();

            float ppm();
            float ppmLowPass();

            int workingInt();
            int auxillaryInt();

            float working();
            float auxillary();
            
            float workingZeroed();
            float auxillaryZeroed();


        protected:

            GasSensorParam param_;

            int workingInt_;
            int auxillaryInt_;

            float working_;
            float auxillary_;

            float workingZeroed_;
            float auxillaryZeroed_;

            float ppb_;
            filter::LowPass ppbLowPassFilter_;

            void initializeValues();
    };


    void analogInputSetup();

} // namespace alphasense


#endif
