// Base class for AHRS algorithms
#ifndef AHRS_h
#define AHRS_h
#include <math.h>
#include <vector>

class AHRS
{
    private:
        float q0, q1, q2, q3;	// quaternion of sensor frame relative to auxiliary frame
        float roll, pitch, yaw;
        float invSampleFreq;

    public:
    	void begin(float sampleFrequency) { invSampleFreq = 1.0f / sampleFrequency; }
        void update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
        void updateIMU(float gx, float gy, float gz, float ax, float ay, float az);
        float getRoll();
        float getPitch();
        float getYaw();
        float getRollRadians();
        float getPitchRadians();
        float getYawRadians();
        std::vector<double> getQuarternion();
};
#endif