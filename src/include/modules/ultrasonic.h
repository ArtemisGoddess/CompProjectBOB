#pragma once

#include "constants.h"

class ultrasonic {
    private:
        int wallDetections = 0; //Wall detection int

        // Warning system for if bot is about to hit a wall. Used by detectWall.
        void crashImminent(int &warning);
    public:
        //Ultrasonic object to interact with detections
        ultrasonic();

        //Distance detection function. Has an attempts system for accuracy.
        int getDistance();

        //Detects the proximity to a wall.
        bool detectWall();
};