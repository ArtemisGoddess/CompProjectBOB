#include "constants.h"
#include <Wire.h>

class gyro {
    private:
        int16_t gyroZ; // Raw gyro Z-axis reading
        float gyroZOffset = 0; // Calibration offset
        float LAST_ANGLE = 0; // Last angle
        float CURRENT_ANGLE = 0; // Current angle
        unsigned long LAST_READ = 0; //Time since last read

    public:
        //The main gyro object. Sets up the gyro.
        gyro();

        // Basic Gyro setup, ensures it is enabled and configured properly
        bool setupGyro();

        // Calibrate gyro (robot must be stationary!)
        void calibrateGyro();

        //Reads the Z value from the Gyro, or the Yaw
        int16_t readGyroZ();

        // Updates all gyroscope angles
        void updateGyroAngle();

        // Reset angles
        void resetAngle();

        // Get current angle
        float getCurrentAngle();

        float getLastAngle();
};