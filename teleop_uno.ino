#include <time.h>
#include <Servo.h>
#include "DS_interface.h"
#include "HbridgeDriveTrain.h"

#define RX_FROM_NMCU 2
#define TX_TO_NMCU 3

#define SERVO_PIN 9
#define PHOTO_SENSOR_PIN A0

#define DRIVE_SPEED 1
#define AUTONOMOUS_DONE 15*1000
#define COLOR_CHANGE_TRESHOLD 780

// ---------------------------------------------------------------------------
/* 
 * Create a Drive Station Interface "DS_Interface" object
 */
DS_Interface ds(RX_FROM_NMCU, TX_TO_NMCU);

/*
 * Drive train object for moving the wheels.
 */
HbridgeDriveTrain drivetrain;
// HbridgeDriveTrain::Direction current_direction =  HbridgeDriveTrain::STOP;

/*
 * Servo for lowering and raising the arm
 */
 Servo armServo;

/* 
 * Arduino Setup where we initialize subsystems
 */
bool inAutonomous = true;

void setup() {
  Serial.begin(115200);
  Serial.println("Interfacing arduino with nodemcu");

  ds.init();     // setup drive station comms 
  drivetrain.init();  // setup the drive train
  stopWheels();  // start with the wheels stopped

  armServo.attach(SERVO_PIN);
}

/* 
 * Arduino loop - where we get input and invoke our program’s
 * subsystems
 */
void loop() {

    char input = ds.readInputIfAvailable();

    if (inAutonomous) {
      runAutonomous();
    }
    
    switch (input) {
      case 't':
        testWheels();
        break;
      case 'w':
        driveForward();
        break;
      case 'a':
        turnRight();
        break;
      case 'd':
        turnLeft();
        break;
      case 's':
        stopWheels();
        break;
      case 'x':
        driveReverse();
        break;
      case 'r':
        testRightWheel();
        break;
      case 'l':
        testLeftWheel();
        break;
      case 'o':
        armUp();
        break;
      case 'p':
        armDown();
        break;
      default:
        break;
    }
}

void runAutonomous() {
    int photoSensor = analogRead(PHOTO_SENSOR_PIN);
    int currentTime = millis();
    Serial.print("Photo sensor value:");
    Serial.println(photoSensor);
    // Serial.print("Current Time:");
    // Serial.println(currentTime);

    if (currentTime > AUTONOMOUS_DONE) {
      stopAutonomous();
      return;
    }

    if (photoSensor > COLOR_CHANGE_TRESHOLD) {
      driveForward();
    }
    else {
      stopAutonomous();
    }
}

void stopAutonomous() {
  stopWheels();
  inAutonomous = false;
}

void armUp() {
  armServo.write(90);
}

void armDown() {
  armServo.write(0);
}

void driveForward() {
  drivetrain.setMotorsDirectly(DRIVE_SPEED, DRIVE_SPEED);
}

void driveReverse() {
  drivetrain.setMotorsDirectly(-DRIVE_SPEED, -DRIVE_SPEED);
}

void turnLeft() {
  drivetrain.setMotorsDirectly(0, DRIVE_SPEED);
}

void turnRight() {
  drivetrain.setMotorsDirectly(DRIVE_SPEED, 0);
}

void stopWheels() {
  drivetrain.setMotorsDirectly(0, 0);
}

void testWheels()
{
    testRightWheel();
    delay(1000);
    stopWheels();
    delay(500);
    testLeftWheel();
    delay(1000);
    stopWheels();
    delay(500);
    driveForward();
    delay(1000);
    stopWheels();
    delay(500);
    driveReverse();
    delay(1000);
    stopWheels();
}

void testRightWheel() {
  drivetrain.setMotorsDirectly(0, DRIVE_SPEED);
}
void testLeftWheel() {
  drivetrain.setMotorsDirectly(DRIVE_SPEED, 0);
}