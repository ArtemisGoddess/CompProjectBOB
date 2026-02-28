#include <constants.h>
#include <gyro.h>
#include <movement.h>
#include <lightSensor.h>
#include <ultrasonic.h>


gyro* Gyro; //Central gyro
movement* Drivetrain; //Drive control for the robot
lightSensor* LightSensor;
ultrasonic* Ultrasonic;

int SINCE_WALL = 0; //Counter since it sees a wall.
int COUNT = 0;



/* -------------------------- * SETUP AND LOOP * -------------------------- */



enum STATE {
  DRIVE = 0,
  SEE_WALL,
  STOP
};

enum STATE CURRENT_STATE = STOP; //Initilizes the current state to be STOP, which waits until button press to DRIVE.

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  FastLED.addLeds<NEOPIXEL, PIN_LED>(leds, NUM_LEDS);

  digitalWrite(PIN_Motor_SAFTEY, HIGH); // Enable the robot to move
  Serial.begin(9600); // Send information at baud rate 9600

  Gyro = new gyro();
  Drivetrain = new movement(Gyro);
  LightSensor = new lightSensor(constants::colorThreshold, Drivetrain);
  Ultrasonic = new ultrasonic();
}

void loop() { //Main loop function; actual robot running
  Gyro->updateGyroAngle();

  switch (CURRENT_STATE) { // Drives forward, turns 90 degrees right if it spots a wall, stops if it is on a white line (may or may not work)
    case DRIVE:
      Drivetrain->drive(constants::SPEED, true);
      SINCE_WALL--; //Decreases the SINCE_WALL counter, for thje sake of turnByLine.

      if (LightSensor->detectLine()) CURRENT_STATE = SEE_WALL;
      if (Ultrasonic->detectWall()) CURRENT_STATE = STOP;

      break;
    case SEE_WALL:
      LightSensor->turnByLine(SINCE_WALL);
      Drivetrain->updateDriveAngle(); //Sets the target DRIVE_ANGLE to be the current angle

      Drivetrain->drive(constants::SPEED, false); //Reverses away from the wall for 0.5 seconds.
      delay(500);

      Drivetrain->updateDriveAngle(); //Updates the DRIVE_ANGLE again before moving.
      CURRENT_STATE = DRIVE; //Changes state
      break;
    case STOP:
      Drivetrain->stop(); //Stops the robot from moving.

      while (digitalRead(BUTTON) == HIGH) {} //Wait until button press

      CURRENT_STATE = DRIVE;
      break;
  }

  COUNT++;
}
