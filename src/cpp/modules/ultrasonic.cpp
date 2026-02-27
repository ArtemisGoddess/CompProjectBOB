#include "ultrasonic.h"

ultrasonic::ultrasonic() {
    pinMode(US_OUT, OUTPUT);
    pinMode(US_IN, INPUT);
}

int ultrasonic::getDistance() {
    int validReading = 0;
    int attempts = 0;

    while (validReading == 0 && attempts < 3) {
        if (attempts > 0) delay(60);  // Only delay on retries


        digitalWrite(US_OUT, LOW);
        delayMicroseconds(2);


        digitalWrite(US_OUT, HIGH);
        delayMicroseconds(10);


        digitalWrite(US_OUT, LOW);


        long duration = pulseIn(US_IN, HIGH, 30000);
        int distance = duration * 0.034 / 2;


        if (duration > 0 && distance <= 800) {
        validReading = distance;
        }


        attempts++;
    }


    return validReading;
}

void ultrasonic::crashImminent(int &warning){ // Warning system for if bot is about to hit a wall
    if (getDistance() <= constants::dangerDistance){
        warning++;
    } else {
        warning = 0;
    }
}

bool ultrasonic::detectWall() {
    this->wallDetections = crashImminent(this->wallDetections);

    if (this->wallDetections >= 3){ //Sees enough wall, so theres a wall
        this->wallDetections = 0;
        return true;
    }

    return false; // No wall
}