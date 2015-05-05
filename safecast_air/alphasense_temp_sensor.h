#ifndef ALPHASENSE_TEMP_SENSOR_H
#define ALPHASENSE_TEMP_SENSOR_H

namespace alphasense
{

    struct TempSensorParam
    {
    };

    class TempSensor
    {
        public:

            TempSensor(TempSensorParam param);

        protected:
            TempSensorParam param_;

    };

}

#endif
