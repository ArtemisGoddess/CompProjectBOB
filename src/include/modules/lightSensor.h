#pragma once

#include "constants.h"
#include "movement.h"

class lightSensor {
    private:
        int* colorThreshold[3];

        movement localDrivetrain;
    public:
        // Light sensor object to interact with detections
        lightSensor(int* colorThreshold[3], movement &Drivetrain);

        //Follows a line at a given speed and turnStrength.
        void followLine(int speed, int turnStrength);

        //Detects if the color sensor sees light with given thresholds and returns a boolean.
        bool detectLine();

        //Wall handling for maze-solving
        void turnByLine(int &timeSinceWall);
};