#include "HbridgeDriveTrain.h"


/*
 * pinout of the hbridge controls on the shield
 */
#define E1 10 // energy level to motor 1
#define E2 11 // energy level to motor 2
#define M1 12 // direction of motor 1
#define M2 13 // direction of motor 2

/*
 * Constructor to create a new drivetrain object
 * leftPin - the index of the DIO pin delivering PWM signal to the left servo
 * rightPin - the index of the DIO pin delivering PWM signal to the right servo
 */
HbridgeDriveTrain::HbridgeDriveTrain() {
    left_motor.speed_pin = E1;
    left_motor.direction_pin = M1;
    right_motor.speed_pin = E2;
    right_motor.direction_pin = M2;
}

/*
 * A destructor so the compiler has instructions for how to clean up
 * when a HbridgeDriveTrain object is destroyed. Program should never
 * invoke in directly.
 */
HbridgeDriveTrain::~HbridgeDriveTrain() {
    left_motor.speed_pin = 0;
    left_motor.direction_pin = 0;
    right_motor.speed_pin = 0;
    right_motor.direction_pin = 0;

    current_direction = STOP;
}

/*
 * init()
 * This function should be called from the arduino "setup()" function
 */
void HbridgeDriveTrain::init() {
    pinMode(left_motor.direction_pin, OUTPUT);
    pinMode(right_motor.direction_pin, OUTPUT); 
    current_direction = STOP;
    service(current_direction);
}

/*
 * Anyone could write a motor class with an "invert()" operation, instead
 * of the simple struct used right now and this wouldn't be needed. The 
 * 2 different definitions of "FWD" and "REV" are  because the wheels and 
 * motors face opposite directions, so what is FORWARD
 * for one is REVERSE for the other. Altenatively the wires could be 
 * reversed on the motors and then this also wouldn't be needed. If wired 
 * way, then "HIGH" would mean forward for both motors and FWD could just
 * be "HIGH".
 */
#define FWD_L HIGH
#define FWD_R LOW
#define REV_L LOW
#define REV_R HIGH
#define FULL_POWER 255
#define NO_POWER 0

void HbridgeDriveTrain::move_motor(HbridgeDriveTrain::Motor &motor, int direction, int pwr) {
    digitalWrite(motor.direction_pin, direction);
    analogWrite(motor.speed_pin, pwr);
}

/*
 * service()
 * Call this function to change the state of the servos. Pass in a Direction
 * wayToGo - is the direction you'd like the robot to go, inlcuding "HbridgeDriveTrain::STOP"
 */
void HbridgeDriveTrain::service(HbridgeDriveTrain::Direction wayToGo) {
    switch(wayToGo) {
        case FORWARD:
            move_motor(left_motor, FWD_L, FULL_POWER);
            move_motor(right_motor, FWD_R, FULL_POWER);
            current_direction = wayToGo;
            break;
        case REVERSE:
            move_motor(left_motor, REV_L, FULL_POWER);
            move_motor(right_motor, REV_R, FULL_POWER);
            current_direction = wayToGo;
            break;
        case LEFT:
            move_motor(left_motor, REV_L, FULL_POWER);
            move_motor(right_motor, FWD_R, FULL_POWER);
            current_direction = wayToGo;
            break;
        case RIGHT:
            move_motor(left_motor, FWD_L, FULL_POWER);
            move_motor(right_motor, REV_R, FULL_POWER);
            current_direction = wayToGo;
            break;
        case STOP:
            move_motor(left_motor, FWD_L, NO_POWER);
            move_motor(right_motor, FWD_R, NO_POWER);
            current_direction = wayToGo;
            break;
    }
}

#define MAX_SPEED 255
void compute_motor_params_from_float(float speed, int& pwm_speed, int& fwd) {
    fwd = LOW;
    if (speed > 0) {
        fwd = HIGH;
    }
    pwm_speed = abs(speed * MAX_SPEED);
}
/*
 * setMotorsDirectly is a way to have direct control of each motor's speed
 */
void HbridgeDriveTrain::setMotorsDirectly(float left_speed, float right_speed) {

    int l_speed;
    int l_fwd;
    int r_speed;
    int r_fwd;
    
    compute_motor_params_from_float(left_speed, l_speed, l_fwd);
    compute_motor_params_from_float(right_speed, r_speed, r_fwd);

    /* 
     * need to invert left motor like we do in service()
     */
    if (HIGH == l_fwd) {
        l_fwd = LOW;
    } else {
        l_fwd = HIGH;
    }

    move_motor(left_motor, l_fwd, l_speed);
    move_motor(right_motor, r_fwd, r_speed);

    if (left_speed > right_speed) {
        current_direction = HbridgeDriveTrain::RIGHT;
    } else if (right_speed > left_speed) {
        current_direction = HbridgeDriveTrain::LEFT;
    } if (left_speed > 0.0) {
        current_direction = HbridgeDriveTrain::FORWARD;
    } if (left_speed < 0.0) {
        current_direction = HbridgeDriveTrain::REVERSE;
    } else {
        current_direction = HbridgeDriveTrain::STOP;
    }
}

HbridgeDriveTrain::Direction HbridgeDriveTrain::getCurrentDirection() {
    return (current_direction);
}