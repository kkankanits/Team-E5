#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHMotor.h>
#include <FEHIO.h>
#include <cmath>
#include <math.h>

#define WHEEL_Radius 1.25
#define TRANSLATIONS_PER_REV 318.0
#define PI 3.14

FEHMotor leftTire(FEHMotor::Motor1, 9);
FEHMotor rightTire(FEHMotor::Motor3, 9);
AnalogInputPin cds(FEHIO::P3_7);


//Declarations for encoders & motors
DigitalEncoder right_encoder(FEHIO::P0_0);
DigitalEncoder left_encoder(FEHIO::P0_1);
FEHMotor right_motor(FEHMotor::Motor3,9.0);
FEHMotor left_motor(FEHMotor::Motor1,9.0);

/* This function waits for the red start light to trigger the robot to start */
void waitForStartLight() {
    //detect any light (will have to change to red)
    while(cds.Value() >= 2.0) {

    }
}
/*
// This function programmed the robot for checkpoint 1 using distance 
void checkpoint1A() {
    //wait for the start light
    waitForStartLight();
    
    //move the robot out the starting position
    moveForwardDistance(20);

    //turn left (halfway) to drive toward the ramp
    turnLeft(.37);

    //move forward to reach the ramp
    moveForwardDistance(23);
    
    //turn right (90 degrees) to face the ramp
    turnRight(.72);

    //drive forward up the ramp 
    moveForwardDistance(46);

    //turn right (90 degree) to face the passport stamp
    turnRight(.6);

    //move forward towards the passport stamp
    moveForwardDistance(25);

    //turn left (90 degree) to face the ticket kiosk
    turnLeft(.5);

    //drive towards the ticket kiosk to touch
    moveForwardDistance(33);

    //drive backward towards the luggage
    moveBackwardDistance(25);

    //turn right backward to face the wall of the robot course
    turnRightBackward(.65);

    //drive to be in front of top of ramp
    moveForwardDistance(18.5);
    
    //drive backward towards the passport stamp to make space for turning
    //moveBackwardDistance(1.5);

    //turn left (90 degree) to face the ramp
    turnLeft(.705);

    //turnLeftBackward(0.7);

    //drive down the ramp
    moveForwardDistance(45);

    //stop
    stop();
}
*/

int distance_to_count(int distance){
    return (distance*318)/(2*M_PI*1.25);
}

void move_forward(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void turn_right(int percent, int counts) //using encoders
{

    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    //hint: set right motor backwards, left motor forwards
    right_motor.SetPercent(-1*percent);
    left_motor.SetPercent(percent);


    //While the average of the left and right encoder is less than counts, keep running motors

    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();

}

void turn_left(int percent, int counts) //using encoders
{

    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    //hint: set right motor backwards, left motor forwards
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-1*percent);


    //While the average of the left and right encoder is less than counts, keep running motors

    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();

}

void checkpoint2()
{
     
}

int main(void)
{
    // while(true) {
    //     LCD.Write(cds.Value());
    //     Sleep(1.0);
    //     LCD.Clear(BLACK);
        
    // }

    // test comment
    LCD.Clear(BLACK);

    return 0;

}