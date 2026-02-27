#pragma once

#include <FastLED.h>

#define PIN_Motor_R 5 // Right
#define PIN_Motor_L 6 // Left
#define PIN_Motor_STEER_R 7 // Direction the wheels move. Right direction
#define PIN_Motor_STEER_L 8 // Left direction
#define PIN_Motor_SAFTEY 3 // Movement safety. I.e, if they're allowed to move.

#define PIN_LED 4 // LED
#define NUM_LEDS 1 // Number of LEDS

#define US_OUT 13 // Ultrasonic Sensor Input
#define US_IN 12 // Ultrasonic Sensor Output

#define Left A2 //Left color sensor
#define Middle A1 //Middle color sensor
#define Right A0 //Right color sensor

#define GYRO 0x68 //Gyro pin

#define BUTTON 2 // Push Button

class constants {
    public:
        int SPEED = 60; //Base movement speed

        int dangerDistance = 5;  // Distance in centimeters required to increment warning
        
        CRGB leds[NUM_LEDS]; //Led object

        int* colorThreshold[3] = {100, 70, 70}; //Thresholds for seeing lines
};

class dynamicConstants {
    public:
        int COUNT = 0;

        enum STATE {
            DRIVE = 0,
            SEE_WALL,
            STOP
        };

        enum STATE CURRENT_STATE = STOP; //Initilizes the current state to be STOP, which waits until button press to DRIVE.

};