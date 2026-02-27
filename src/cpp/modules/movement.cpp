#include "movement.h"

movement::movement(gyro &Gyro) {
    this->localGyro = Gyro;

    pinMode(PIN_Motor_R, OUTPUT); // Motor-pin assignment
    pinMode(PIN_Motor_L, OUTPUT);
    pinMode(PIN_Motor_STEER_R, OUTPUT);
    pinMode(PIN_Motor_STEER_L, OUTPUT);
    pinMode(PIN_Motor_SAFTEY, OUTPUT);
}

void movement::drive(int speed, bool forward) {
    uint8_t dir = (forward) ? HIGH : LOW; // Direction boolean translator. True is forward, false is backwards

    digitalWrite(PIN_Motor_STEER_L, dir);
    digitalWrite(PIN_Motor_STEER_R, dir); //Direction

    if (abs(DRIVE_ANGLE - localGyro.getCurrentAngle()) <= 1) { //If the absolute drift from the initial DRIVE_ANGLE is over 1 degree, adjust.
        turn(DRIVE_ANGLE - localGyro.getCurrentAngle(), 0.2); // Turn to until within 0.2 degrees of the initial DRIVE_ANGLE
    } else {
        analogWrite(PIN_Motor_R, speed); // Speed
        analogWrite(PIN_Motor_L, speed);
    }
}

void movement::stop() {
    analogWrite(PIN_Motor_R, 0);
    analogWrite(PIN_Motor_L, 0);
}

void movement::setAngle(float angle, float tolerance) {
    float posDiff = fmod((angle - localGyro.getCurrentAngle()), 360.0);
    float negDiff = fmod((angle + localGyro.getCurrentAngle()), 360.0);
    uint8_t dir = (abs(negDiff) > abs(posDiff)) ? LOW : HIGH; //Checks which direction is most efficient, and takes it

    digitalWrite(PIN_Motor_STEER_R, dir);
    digitalWrite(PIN_Motor_STEER_L, dir ^ 1U); // Bit-flip to go opposite to the right wheels

    while (abs(localGyro.getCurrentAngle() - angle) > tolerance) { // While not within the acceptable tolerance range, turn.
        localGyro.updateGyroAngle(); //Updates the angle every loop to ensure accuracy.

        int speed = min(max(log((abs(localGyro.getCurrentAngle() - angle) / 5) + 1) * 90, constants::SPEED), 100); //Exponential speed equation, hits SPEED when it gets close the needed value, and maxes out at 100. Makes turning fast.

        analogWrite(PIN_Motor_R, speed);
        analogWrite(PIN_Motor_L, speed);
    }

    stop();
}

void movement::turn(float angle) {
    setAngle(fmod(localGyro.getCurrentAngle() + angle, 360.0), 2);
}

void movement::turn(float angle, float thres) {
    setAngle(fmod(localGyro.getCurrentAngle() + angle, 360.0), thres);
}

void movement::updateDriveAngle() {
    DRIVE_ANGLE = localGyro.getCurrentAngle();
}