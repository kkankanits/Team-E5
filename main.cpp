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
    while(cds.Value() >= 2.0);
}

int distance_to_count(int distance){
    return (distance*TRANSLATIONS_PER_REV)/(2*M_PI*WHEEL_Radius);
}

void move_forward(int percent, int counts, float timeout) //using encoders 
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    float start = TimeNow();

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((((left_encoder.Counts() + right_encoder.Counts()) / 2.) < counts) && (TimeNow() - start < timeout));

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void move_backward(int percent, int counts, float timeout)
{
    move_forward(-1*percent, counts, timeout);
}

void turn_right(int percent, int counts, float timeout) //using encoders 50, 250 for 90 degree
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //set right motor backwards, left motor forwards
    right_motor.SetPercent(-1*percent);
    left_motor.SetPercent(percent);

    float start = TimeNow();

    //While the average of the left and right encoder is less than counts, keep running motors
    while(((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts) && (TimeNow() - start < timeout));

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();

}

void turn_left(int percent, int counts, float timeout) //using encoders 50, 250 for 90 degrees
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //set right motor backwards, left motor forwards
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-1*percent);

    float start = TimeNow();

    //While the average of the left and right encoder is less than counts, keep running motors
    while(((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts) && (TimeNow() - start < timeout));

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();

}

//true is red, false is blue
bool isRedLight() {
    //blue is less than 2, red is less than 1
    //LCD.Write(cds.Value());
    if(cds.Value() <= 1.15) {
        return 1;
    }
    else {
        return 0;
    }
}

void checkpoint2()
{
    waitForStartLight();
    Sleep(0.5);

    //hit the start button
    move_backward(25, distance_to_count(1.8), 2.);

    // get out of start light area
    move_forward(25, distance_to_count(16), 15.);
    
    // turn left facing the fuel switches
    turn_left(50, 120, 5.);

    // forward towards the left steeper ramp
    move_forward(25, distance_to_count(10.8), 15.);

    // turn right to face ramp
    turn_right(50, 325, 10.0);

    // get up ramp and closer to color light
    move_forward(25, distance_to_count(27), 20.);

    //turn right after got up the ramp bc it steer to the left
    turn_right(50, 40, 2.0);

    // get up ramp and closer to color light
    move_forward(25, distance_to_count(9.7), 10.);

    // turn towards light (slight right turn)
    turn_right(50, 170, 5.);

    // move to get the sensor above the color light 
    move_forward(25, distance_to_count(8), 4.);

    Sleep(1.0);

    float distanceToButton;
    int turntoButton;
    float distanceToRamp;
    bool red = 0;

    if(isRedLight()) {
        LCD.Clear(BLACK);
        LCD.Write("RED");
        distanceToButton = 11;
        turntoButton = 250;  
        distanceToRamp = 11;
        red = 1;
    }
    else {
        LCD.Clear(BLACK);
        LCD.Write("BLUE");
        distanceToButton = 12;
        turntoButton = 270;  
        distanceToRamp = 8.5;
    }

    //move backward a little to give space for turning
    move_backward(25, distance_to_count(8), 5.);

    //turn to make the arm face the ticket kiosk
    turn_right(50, 150, 5.);

    //drive towards button
    move_forward(25, distance_to_count(distanceToButton), 10.);

    //turn towards button
    turn_left(50, turntoButton, 5.);

    //drive backward from button (for space)
    move_backward(25, distance_to_count(2), 5.);

    //drive towards button
    move_forward(25, distance_to_count(7), 5.);

    //move backward away from ticket kiosk
    move_backward(25, distance_to_count(10), 15.);

    //turn towards ramp
    turn_left(50, 280, 5.);

    //move forward to ramp
    move_forward(25, distance_to_count(distanceToRamp), 15.);

    //turn left to go down the ramp
    turn_left(50, 275, 5.);

    //move forward down the ramp
    move_forward(25, distance_to_count(15), 30.);

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