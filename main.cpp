#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHMotor.h>
#include <FEHIO.h>
#include <cmath>
#include <math.h>

#define WHEEL_Radius 1.25
#define TRANSLATIONS_PER_REV 318.0


AnalogInputPin cds(FEHIO::P3_7);


//Declarations for encoders & motors
DigitalEncoder right_encoder(FEHIO::P0_0);
DigitalEncoder left_encoder(FEHIO::P0_1);
FEHMotor right_motor(FEHMotor::Motor3,9.0);
FEHMotor left_motor(FEHMotor::Motor1,9.0);

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

/* This function waits for the red start light to trigger the robot to start */
void waitForStartLight() {
    //detect any light (will have to change to red)
    while(cds.Value() >= 2.0) {

    }
}

int distance_to_count(int distance){
    return (distance*TRANSLATIONS_PER_REV)/(2*M_PI*WHEEL_Radius);
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

void move_backward(int percent, int counts)
{
    move_forward(-1*percent, counts);
}

void turn_right(int percent, int counts) //using encoders 50, 250 for 90 degree
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

void turn_left(int percent, int counts) //using encoders 50, 250 for 90 degrees
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

//true is red, false is blue
bool isRedLight() {
    //blue is less than 2, red is less than 1
    //LCD.Write(cds.Value());
    if(cds.Value() <= 1.3) {
        return 1;
    }
    else {
        return 0;
    }
}

void checkpoint2()
{
    waitForStartLight();
    //hit the start button
    move_backward(25, distance_to_count(2));

    // get out of start light area
    move_forward(25, distance_to_count(16));
    
    // turn left facing the fuel switches
    turn_left(50, 150);

    // forward towards the left steeper ramp
    move_forward(25, distance_to_count(10.5));

    // turn right to face ramp
    turn_right(50, 295);

    // get up ramp and closer to color light
    move_forward(25, distance_to_count(36));

    // turn towards light (slight right turn)
    turn_right(50, 155);

    // move to get the sensor above the color light 
    move_forward(25, distance_to_count(8));

    Sleep(1.0);

    if(isRedLight()) {
        LCD.Clear(BLACK);
        LCD.Write("RED");
        //move backward a little to give space for turning
        move_backward(25, distance_to_count(2));
        //turn to make the arm face the ticket kiosk
        turn_right(50, 140);
        //drive towards button
        move_forward(25, distance_to_count(10));
        //turn towards button
        turn_left(50, 295);
    }
    else {
        LCD.Clear(BLACK);
        LCD.Write("BLUE");
        //move backward a little to give space for turning
        move_backward(25, distance_to_count(2));
        //turn to make the arm face the ticket kiosk
        turn_right(50, 140);
        //drive towards button
        move_forward(25, distance_to_count(6));
        //turn towards button
        turn_left(50, 295);
    }

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

    checkpoint2();

    return 0;

}