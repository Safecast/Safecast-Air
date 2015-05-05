#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "alphasense_gas_sensor.h"

namespace constants
{
    const int num_gas_sensor = 6;
    extern const int gas_sensor_ain_res; 
    extern const int gas_sensor_ain_avg;
    extern const float gas_sensor_sample_dt;
    extern const alphasense::GasSensorParam gas_sensor_param[];

}


#endif
