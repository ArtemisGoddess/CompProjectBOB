#include "lightSensor.h"

lightSensor::lightSensor(const int* colorThreshold[3], movement &Drivetrain) {
    this->localDrivetrain = Drivetrain;
    this->colorThreshold = colorThreshold;

    pinMode(Left, INPUT);
    pinMode(Middle, INPUT);
    pinMode(Right, INPUT);
}

void lightSensor::followLine(int speed, int turnStrength) {  
    int leftVal = (this->colorThreshold[0] < analogRead(Left)) ? 1000 : analogRead(Left);
    int middleVal = (this->colorThreshold[1] < analogRead(Middle)) ? 1000 : analogRead(Middle);
    int rightVal = (this->colorThreshold[2] < analogRead(Right)) ? 1000 : analogRead(Right); //Tripple check to ensure read values fall within acceptable ranges, otherwise IGNORE THEM.

    if (abs(leftVal - rightVal) < 10) { //If the difference between the left and right detections is within 10, ignore them. Means they both see a line.
        leftVal = 1000;
        rightVal = 1000;
    }

    if (middleVal < this->colorThreshold[0]) { //Secondary check to do actions relative to what isn't ignored.
        this->localDrivetrain.drive(speed, true);
    } else {
        if (leftVal < this->colorThreshold[1]) {
            this->localDrivetrain.turn(-turnStrength, float(turnStrength)/5);
        } else if (rightVal < this->colorThreshold[2]) {
            this->localDrivetrain.turn(turnStrength, float(turnStrength)/5);
        }
    }
}

bool lightSensor::detectLine() {
    if ((analogRead(Left) < this->colorThreshold[0]) && (analogRead(Middle) < this->colorThreshold[1]) && (analogRead(Right) < this->colorThreshold[2])) return true;
    return false;
}

void lightSensor::turnByLine(int &timeSinceWall) {
    if (timeSinceWall <= 0) {
        this->localDrivetrain.turn(90, 0.5);
        timeSinceWall = 20;
    } else {
        this->localDrivetrain.turn(-180, 0.5);
        timeSinceWall = 0;
    }

}