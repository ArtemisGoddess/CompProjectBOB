#include <constants.h>
#include <gyro.cpp>
#include <lightsensor.cpp>
#include <movement.cpp>
#include <ultrasonic.cpp>
#include <weewoo.cpp>


gyro* Gyro; //Central gyro
movement* Drivetrain; //Drive control for the robot
lightSensor* LightSensor;
ultrasonic* Ultrasonic;

int SINCE_WALL = 0; //Counter since it sees a wall.
int SINCE_TURN = 0;
int COUNT = 0;



/* -------------------------- * SETUP AND LOOP * -------------------------- */



enum STATE {
  DRIVE = 0,
  SEE_WALL,
  SEARCH_WALLS,
  STOP
};


enum STATE CURRENT_STATE = STOP; //Initilizes the current state to be STOP, which waits until button press to DRIVE.
enum STATE PREVIOUS_STATE = CURRENT_STATE; //WORK IN PROGRESS

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  FastLED.addLeds<NEOPIXEL, PIN_LED>(leds, NUM_LEDS);

  digitalWrite(PIN_Motor_SAFTEY, HIGH); // Enable the robot to move
  Serial.begin(9600); // Send information at baud rate 9600

  Gyro = new gyro();
  Ultrasonic = new ultrasonic();
  Drivetrain = new movement(Gyro, Ultrasonic);
  LightSensor = new lightSensor(constants::colorThreshold, constants::colorThresholdInv, Drivetrain);
}

void loop() { //Main loop function; actual robot running
  Gyro->updateGyroAngle();


  switch (CURRENT_STATE) { // -----------------------------------------------------------
    case DRIVE:
      weeWoo::setRGB(CRGB::Purple);
      
      SINCE_WALL--; //Decreases the SINCE_WALL counter, for thje sake of turnByLine.
      SINCE_SECONDARY_WALL--;
      PREVIOUS_STATE = DRIVE;

      if (Ultrasonic->detectWall()) CURRENT_STATE = SEE_WALL;
      else if (LightSensor->detectLine()) CURRENT_STATE = LINE_FOLLOWING;

      if (Ultrasonic->detectWall()) CURRENT_STATE = STOP;
      if (LightSensor->detectLine()) CURRENT_STATE = SEE_WALL;
      if (SINCE_TURN >= constants::detectDistance) CURRENT_STATE = SEARCH_WALLS;

      SINCE_TURN++;

      break;
    case SEE_WALL:
      SINCE_TURN = 0;
      LightSensor->turnByLine(SINCE_WALL);
      Drivetrain->updateDriveAngle(); //Sets the target DRIVE_ANGLE to be the current angle

      Drivetrain->drive(constants::SPEED, false); //Reverses away from the wall for 0.5 seconds.
      delay(500);

      Drivetrain->updateDriveAngle(); //Updates the DRIVE_ANGLE again before moving.
      CURRENT_STATE = DRIVE; //Changes state
      break;
    case SEARCH_WALLS:
      SINCE_TURN = 0;
      Drivetrain->turn(90);
      CURRENT_STATE = DRIVE;

      break;
    case STOP:
      SINCE_TURN = 0;
      Drivetrain->stop(); //Stops the robot from moving.

      while (digitalRead(BUTTON) == HIGH) {} //Wait until button press
      delay(1000);
      CURRENT_STATE = DRIVE;
      break;
  }

}
