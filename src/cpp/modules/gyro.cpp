#include "gyro.h"

gyro::gyro() {
    if (!setupGyro()) { //Gyro setup. Turns red if it fails miserably!
        constants::leds[0] = CRGB::Red;
        while (true);  // Hard stop
    }

    calibrateGyro();
}

bool gyro::setupGyro() {
    Wire.begin();
    Wire.beginTransmission(GYRO);
    Wire.write(0x6B);  // PWR_MGMT_1 register
    Wire.write(0);     // Wake up MPU6050

    byte error = Wire.endTransmission();

    if (error != 0) {
        return false;
    }

    // Configure gyro sensitivity (±250 deg/s)
    Wire.beginTransmission(GYRO);
    Wire.write(0x1B);  // GYRO_CONFIG register
    Wire.write(0x00);  // ±250 deg/s
    Wire.endTransmission();


    LAST_READ = millis();
    return true; //Confirms sucessful setup
}

void gyro::calibrateGyro() {
    delay(500); //Keep it still
    long sum = 0;
    int samples = 100;

    for (int i = 0; i < samples; i++) {
        Wire.beginTransmission(GYRO);
        Wire.write(0x47);  // GYRO_ZOUT_H register
        Wire.endTransmission(false);
        Wire.requestFrom(GYRO, 2, true);
        int16_t gz = Wire.read() << 8 | Wire.read();
        sum += gz;
        delay(10);
    }

    gyroZOffset = sum / samples;
    CURRENT_ANGLE = 0;
}

int16_t gyro::readGyroZ() {
    Wire.beginTransmission(GYRO);
    Wire.write(0x47);  // GYRO_ZOUT_H register
    Wire.endTransmission(false);
    Wire.requestFrom(GYRO, 2, true);

    int16_t gz = Wire.read() << 8 | Wire.read();

    return gz;
}

void gyro::updateGyroAngle() {
    unsigned long now = millis();
    float dt = (now - LAST_READ) / 1000.0;  // Time in seconds

    LAST_READ = now;

    // Read gyro
    gyroZ = readGyroZ();

    // Convert to degrees per second (sensitivity = 131 for ±250 deg/s)
    // INVERTED THE SIGN HERE to fix direction!
    float gyroRate = -((gyroZ - gyroZOffset) / 131.0);

    // Integrate to get angle. Set the LAST_ANGLE to what CURRENT_ANGLE is before integration.
    LAST_ANGLE = CURRENT_ANGLE;
    CURRENT_ANGLE += gyroRate * dt;

    // Keep angle in range of 0 to 360 (easier for rotational math)
    CURRENT_ANGLE = fmod(CURRENT_ANGLE, 360.0);
}

void gyro::resetAngle() {
    CURRENT_ANGLE = 0;
    LAST_ANGLE = 0;
}

float gyro::getCurrentAngle() {
    return CURRENT_ANGLE;
}

float gyro::getLastAngle() {
    return LAST_ANGLE;
}