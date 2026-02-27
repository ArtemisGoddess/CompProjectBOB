#pragma once

#include "constants.h"
#include "gyro.h"

class movement {
    private:
        float DRIVE_ANGLE = 0; // Angle the robot is driving at
        gyro localGyro;
    public:
        movement();
        //main movement controller
        movement(gyro &Gyro);

        // Drive given speed and if forwards or backwards
        void drive(int speed, bool forward);

        // Stops the robot from moving
        void stop();

        //Sets the angle relative to the starting calibration.
        void setAngle(float angle, float tolerance);

        // Turns right if the angle is positive, turns left if the angle is negative.
        void turn(float angle);

        // Overload of turn. Has a threshold option for fine-tuning.
        void turn(float angle, float thres);

        // Updates the current drive angle
        void updateDriveAngle();
};  