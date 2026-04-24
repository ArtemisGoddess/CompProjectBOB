#include <../../../../../../../../../../OneDrive/Documents/Arduino/Codes/CompProjectBOB/main/libraries/constants/constants.h>
#include <../../../../../../../../../../OneDrive/Documents/Arduino/Codes/CompProjectBOB/main/libraries/gyro/gyro.cpp>
#include <../../../../../../../../../../OneDrive/Documents/Arduino/Codes/CompProjectBOB/main/libraries/lightsensor/lightsensor.cpp>
#include <../../../../../../../../../../OneDrive/Documents/Arduino/Codes/CompProjectBOB/main/libraries/movement/movement.cpp>
#include <../../../../../../../../../../OneDrive/Documents/Arduino/Codes/CompProjectBOB/main/libraries/ultrasonic/ultrasonic.cpp>
#include <../../../../../../../../../../OneDrive/Documents/Arduino/Codes/CompProjectBOB/main/libraries/weewoo/weewoo.cpp>


gyro* Gyro; //Central gyro
movement* Drivetrain; //Drive control for the robot
lightSensor* LightSensor;
ultrasonic* Ultrasonic;

int SINCE_WALL = 0; //Counter since it sees a wall.
int SINCE_SECONDARY_WALL = 0;
int INV_COUNTER = 0;



/* -------------------------- * SETUP AND LOOP * -------------------------- */



enum STATE {
  DRIVE = 0,
  SEE_WALL,
  LINE_FOLLOWING,
  DANCE,
  STOP,
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
      else if (LightSensor->detectInv() && INV_COUNTER > 0){
        Drivetrain->turn(180);
        Drivetrain->updateDriveAngle();
      } 

      Drivetrain->drive(constants::SPEED, true);

      break;
    case SEE_WALL: // -----------------------------------------------------------
      weeWoo::setRGB(CRGB::Green);
      Drivetrain->turnProcess();
      Drivetrain->updateDriveAngle(); //Sets the target DRIVE_ANGLE to be the current angle

      CURRENT_STATE = PREVIOUS_STATE; //Changes state

      break;
    case LINE_FOLLOWING: // -----------------------------------------------------------
      LightSensor->followLine();
      weeWoo::setRGB(CRGB::Yellow);

      if (LightSensor->detectInv() && INV_COUNTER == 0){
        INV_COUNTER++;
        Drivetrain->updateDriveAngleTrue();
        Drivetrain->drive(constants::SPEED, true);
        delay(500);
        Drivetrain->stop();
        CURRENT_STATE = DRIVE;
      } 
      else if (LightSensor->detectInv()) CURRENT_STATE = DANCE;

      break;
    case DANCE: // -----------------------------------------------------------
      weeWoo::setRGB(CRGB::HotPink);
      Drivetrain->dance();

      CURRENT_STATE = STOP;
    
    case STOP: // -----------------------------------------------------------
      weeWoo::setRGB(CRGB::Blue);
      Drivetrain->stop(); //Stops the robot from moving.

      while (digitalRead(BUTTON) == HIGH) {} //Wait until button press
      delay(1000);
      CURRENT_STATE = DRIVE;
      break;
  }

}
