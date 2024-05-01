#ifndef _IMU_SENSOR_VIEW0_H_
#define _IMU_SENSOR_VIEW0_H_

#define NUM_SENSOR_DATA 10

class View0
{
    public:
        float sensorData[NUM_SENSOR_DATA];

        // shimple view
        void show(float roll, float pitch, float yaw);

        // detail view
        void showDetail(float gyroX, float gyroY, float gyroZ,
                        float accX, float accY, float accZ,
                        float roll, float pitch, float yaw,
                        float temp);
};


#endif  // _IMU_SENSOR_VIEW0_H_
