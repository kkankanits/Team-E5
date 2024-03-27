#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHMotor.h>
#include <FEHBattery.h>
#include <FEHIO.h>
#include <FEHRCS.h>
#include <cmath>
#include <math.h>

#define WHEEL_Radius 1.25
#define TRANSLATIONS_PER_REV 318.0
#define BATTERY_FULL 11.5
#define SERVO_MIN 500
#define SERVO_MAX 2270

//Declaration for CdS cell
AnalogInputPin cds(FEHIO::P3_7);
//Declarations for encoders & motors
DigitalEncoder right_encoder(FEHIO::P0_0);
DigitalEncoder left_encoder(FEHIO::P0_1);
FEHMotor right_motor(FEHMotor::Motor3,9.0);
FEHMotor left_motor(FEHMotor::Motor1,9.0);
//Declaration for servo motor
FEHServo arm_servo(FEHServo::Servo7); 

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
    Sleep(0.5);
    //detect any light (will have to change to red)
    while(cds.Value() >= 2.0);
}

int distance_to_count(int distance){
    return (distance*TRANSLATIONS_PER_REV)/(2*M_PI*WHEEL_Radius);
}

void move_forward(float percent, int counts, float timeout) //using encoders 
{
    percent = (BATTERY_FULL / Battery.Voltage()) * percent;
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

void move_backward(float percent, int counts, float timeout)
{
    move_forward(-1*percent, counts, timeout);
}

void turn_right(float percent, int counts, float timeout) //using encoders 50, 250 for 90 degree
{
    percent = (BATTERY_FULL / Battery.Voltage()) * percent;
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

void turn_left(float percent, int counts, float timeout) //using encoders 50, 250 for 90 degrees
{
    percent = (BATTERY_FULL / Battery.Voltage()) * percent;
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

void setMinMaxServo() {
    arm_servo.SetMin(SERVO_MIN);
    arm_servo.SetMax(SERVO_MAX);
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
    turn_right(50, 20, 2.0);

    // get up ramp and closer to color light
    move_forward(25, distance_to_count(10), 10.);

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
        distanceToButton = 13;
        turntoButton = 170;  
        distanceToRamp = 13;
        red = 1;
    }
    else {
        LCD.Clear(BLACK);
        LCD.Write("BLUE");
        distanceToButton = 12;
        turntoButton = 275;  
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

    if(red){
        //move forward toward passport stamp
        move_forward(25, distance_to_count(4.2), 5.);

        //turn left to face the button
        turn_left(50, 105, 3.);
    }

    //drive backward from button (for space)
    move_backward(25, distance_to_count(2), 5.);

    //drive towards button
    move_forward(25, distance_to_count(8.5), 5.);

    //move backward away from ticket kiosk
    move_backward(25, distance_to_count(10), 15.);

    //turn towards ramp
    turn_left(50, 280, 5.);

    //move forward to ramp
    move_forward(25, distance_to_count(distanceToRamp), 15.);

    //split the code to prevent the robot from hitting the wall

    //turn left halfway to go down the ramp
    turn_left(50, 205, 5.);

    //move forward to ramp
    move_forward(25, distance_to_count(4.5), 5.);

    //turn left to go down the ramp
    turn_left(50, 35, 5.);

    //move forward down the ramp
    move_forward(25, distance_to_count(20), 20.);

}

void checkpoint3()
{
    //set servo motor min and mac
    setMinMaxServo();

    //reset servo motor
    arm_servo.SetDegree(30);

    // Initialize the RCS
    RCS.InitializeTouchMenu("E5NPDU9yC");

    //wait for start light
    Sleep(0.5);
    waitForStartLight();

    //hit the start button
    move_backward(25, distance_to_count(3.5), 1.5);

    // get out of start light area
    move_forward(25, distance_to_count(21.5), 5.);
    
    // turn left toward the fuel switches
    turn_left(50, 110, 3.);

    // Get correct lever from the RCS
    int correctLever = 0;
    //int correctLever = RCS.GetCorrectLever();
    LCD.Write(correctLever);
    //A = 4.5
    float distanceToLever;
     
    // Check which lever to flip and perform some action
    if(correctLever == 0)
    {
        // Perform actions to flip left lever
        distanceToLever = 4.5;
    } 
    else if(correctLever == 1)
    {
        // Perform actions to flip middle lever
        distanceToLever = 7.5;
    }
    else if(correctLever == 2)
    {
       // Perform actions to flip right lever
       distanceToLever = 9;
    }

    // move to the fuel lever
    move_forward(25, distance_to_count(distanceToLever), 8.);

    // turn left facing the fuel switches
    turn_left(50, 200, 3.);

    // back up a little to give space
    move_backward(25, distance_to_count(3.8), 3.);

    float start = TimeNow();
    //timeout if servo doesnt move
    while(TimeNow() - start < 2.0) {
        //flip the lever down
        arm_servo.SetDegree(143);
    }
    
    //reverse backward 
    move_backward(25, distance_to_count(3), 3.);

    //sleep for 5 sec
    Sleep(7.0);

    //move the servo down a bit
    arm_servo.SetDegree(160);

    //move up to the lever
    move_forward(25, distance_to_count(3), 3.);

    //lift the lever back up
    arm_servo.SetDegree(140);

    // move up a little to give space
    move_backward(25, distance_to_count(3), 3.);

    
}

void checkpoint4() {
    //set servo motor min and mac
    setMinMaxServo();

    //reset servo motor
    arm_servo.SetDegree(30);

    //wait for start light
    waitForStartLight();

    //hit the start button
    move_backward(25, distance_to_count(3.5), 1.3);

    // get out of start light area
    move_forward(25, distance_to_count(18), 5.);
    
    // turn left toward the fuel switches
    turn_left(50, 105, 2.);

    // forward towards the left steeper ramp
    move_forward(25, distance_to_count(11), 15.);

    // turn right to face ramp
    turn_right(50, 230, 2.0);

    //line up with the ramp
    move_forward(25, distance_to_count(2), 20.);

    // get up ramp 
    move_forward(40, distance_to_count(10), 20.);

    //turn left slightly on the ramp bc it steer to the right
    turn_right(50, 15, 3.0);

    // get up ramp completely
    move_forward(40, distance_to_count(12), 20.);

    //turn right after got up the ramp to face passport lever
    turn_right(50, 212, 2.0);

    //move to get closer to passport lever
    move_forward(25, distance_to_count(16.2), 10.);

    // turn right to face luggage
    turn_right(50, 230, 3.);

    //backup to get the back of the robot close to ticket kiosk
    move_backward(25, distance_to_count(15), 10.);

    //move the arm down
    arm_servo.SetDegree(178);

    //back up a little
    move_backward(25, distance_to_count(5), 10.);

    //turn the robot left to position the arm under the lever
    turn_left(50, 20, 1.);

    float start = TimeNow();
    //timeout if servo doesnt move
    while(TimeNow() - start < 2.0) {
        //turn the arm up a little to flip it
        arm_servo.SetDegree(110);
    }

    //turn the robot left to position the arm under the lever
    turn_left(50, 10, 1.);

    //back up a little more
    move_backward(25, distance_to_count(.5), 10.);

    //move the arm up a little
     start = TimeNow();
    //timeout if servo doesnt move
    while(TimeNow() - start < 2.0) {
        //turn the arm up a little to flip it
        arm_servo.SetDegree(90);
    }
    //turn left a little more
    turn_left(50, 10, 1.);

    //move the arm up a little
     start = TimeNow();
    //timeout if servo doesnt move
    while(TimeNow() - start < 1.0) {
        //turn the arm up a little to flip it
        arm_servo.SetDegree(80);
    }

    //turn right to leave
    turn_right(50, 40, 1.);



}


int main(void)
{
    // test comment
    LCD.Clear(BLACK);

    //code for checkpoint 4
    checkpoint4();

    return 0;

}