#include "constants.h"
#include "gyro.h"
#include "movement.h"

gyro Gyro; //Central gyro
movement driveControl; //Drive control for the robot

int WALLDETECTED = 0; //Wall detection int




/* -------------------------- * LINE DETECTION * -------------------------- */




void followLine(int* threshold[3], int speed, int turnStrength) { //Follows a line at a given speed and turnStrength.
  int leftVal = (threshold[0] < analogRead(Left)) ? 1000 : analogRead(Left);
  int middleVal = (threshold[1] < analogRead(Middle)) ? 1000 : analogRead(Middle);
  int rightVal = (threshold[2] < analogRead(Right)) ? 1000 : analogRead(Right); //Tripple check to ensure read values fall within acceptable ranges, otherwise IGNORE THEM.

  if (abs(leftVal - rightVal) < 10) { //If the difference between the left and right detections is within 10, ignore them. Means they both see a line.
    leftVal = 1000;
    rightVal = 1000;
  }

  if (middleVal < threshold[0]) { //Secondary check to do actions relative to what isn't ignored.
    drive(speed, true);
  } else {
    if (leftVal < threshold[1]) {
      turn(-turnStrength, float(turnStrength)/5);
    } else if (rightVal < threshold[2]) {
      turn(turnStrength, float(turnStrength)/5);
    }
  }
}

bool detectLine(int* threshold[3]){ //Detects if the color sensor sees light with given thresholds and returns a boolean.
  if ((analogRead(Left) < threshold[0]) && (analogRead(Middle) < threshold[1]) && (analogRead(Right) < threshold[2])){
    return true;
  }

  return false;
}

void turnByLine(int &timeSinceWall) {
  if (timeSinceWall <= 0) {
    turn(90);
    timeSinceWall = 10;
  } else {
    turn(-180);
    timeSinceWall = 0;
  }
}




/* -------------------------- * ULTRASONIC DETECTION * -------------------------- */




int getDistance() { //Distance detection function. Has an attempts system for accuracy.
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

    if (duration > 0 && distance <= 200) {
      validReading = distance;
    }

    attempts++;
  }

  return validReading;
}

void crashImminent(int &warning){ // Warning system for if bot is about to hit a wall
  int dangerDistance = 5;  // Distance in centemeters required to increment warning

  if (getDistance() <= dangerDistance){
    warning++;
  } else {
    warning = 0;
  }
}

bool detectWall(int &wallDetections){ //Detects the proximity to a wall.
  wallDetections = crashImminent(wallDetections); //Uses a global because ONE OF US didn't want to use pass-by-reference (This is lighthearted PFFT)

  if (wallDetections >= 3){ //Sees enough wall, so theres a wall
    wallDetections = 0;
    return true;
  }

  return false; // No wall
}




/* -------------------------- * LED INTERACTION * -------------------------- */




CRGB weeWoo(CRGB ledColour){ // Rapidly changes the led between red and blue to make a weewoo
  if (COUNT % 100 == 0){
    if (ledColour == CRGB::Blue){
      return CRGB::Red;
    }
    else{
      return CRGB::Blue;
    }
  }
}




/* -------------------------- * SETUP AND LOOP * -------------------------- */


int SINCE_WALL = 0; //Counter since it sees a wall.

void setup() {

  pinMode(Left, INPUT);
  pinMode(Middle, INPUT);
  pinMode(Right, INPUT);

  pinMode(US_OUT, OUTPUT);
  pinMode(US_IN, INPUT);

  pinMode(BUTTON, INPUT_PULLUP);

  digitalWrite(PIN_Motor_SAFTEY, HIGH); // Enable the robot to move
  Serial.begin(9600); // Send information at baud rate 9600

  Gyro = new gyro();
  driveControl = new movement(Gyro);
}

void loop() { //Main loop function; actual robot running
  Gyro.updateGyroAngle();
  int* colorThreshold[3] = {100, 70, 70}; //Thresholds for seeing lines

  switch (constants::CURRENT_STATE) { // Drives forward, turns 90 degrees right if it spots a wall, stops if it is on a white line (may or may not work)
    case constants::DRIVE:
      driveControl.drive(constants::SPEED, true);
      SINCE_WALL--; //Decreases the SINCE_WALL counter, for thje sake of turnByLine.

      if (detectLine(colorThreshold)) constants::CURRENT_STATE = constants::SEE_WALL;
      if (detectWall()) constants::CURRENT_STATE = constants::STOP;

      break;
    case constants::SEE_WALL:
      turnByLine(SINCE_WALL);
      driveControl.updateDriveAngle(); //Sets the target DRIVE_ANGLE to be the current angle

      driveControl.drive(constants::SPEED, false); //Reverses away from the wall for 0.5 seconds.
      delay(500);

      DRIVE_ANGLE = Gyro.getCurrentAngle(); //Updates the DRIVE_ANGLE again before moving.
      constants::CURRENT_STATE = constants::DRIVE; //Changes state
      break;
    case constants::STOP:
      driveControl.stop(); //Stops the robot from moving.

      while (digitalRead(BUTTON) == HIGH) {} //Wait until button press

      constants::CURRENT_STATE = constants::DRIVE;
      break;
  }
}
