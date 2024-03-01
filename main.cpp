#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHMotor.h>
#include <FEHIO.h>
#include <cmath>
#include <math.h>

#define WHEEL_Radius 2.5
#define TRANSLATIONS_PER_REV 318
#define PI 3.14
#define WHEEL_CCFR WHEEL_Radius*M_PI

FEHMotor leftTire(FEHMotor::Motor1, 9);
FEHMotor rightTire(FEHMotor::Motor3, 9);
AnalogInputPin cds(FEHIO::P3_7);
//shaft variables
DigitalEncoder rightShaft(FEHIO::P0_0);
DigitalEncoder leftShaft(FEHIO::P0_1);

class Movement{
Movement(int rightSpeed, int leftSpeed)
{
   //default 
}
//given a distance we will calculate whats needed to make the robot move that much
void moveDistance(float distance)
{
//PID tutorial
}
void setRightSpeed(int speed)
{
    rightTire.SetPercent(speed);
}
void setLeftSpeed(int speed)
{
    leftTire.SetPercent(speed);
}
};

/* This function waits for the red start light to trigger the robot to start */
void waitForStartLight() {
    //detect any light (will have to change to red)
    while(cds.Value() >= 2.0) {

    }
}

/* This functions drives the robot forward using time */
void driveForward(float sec) {
    leftTire.SetPercent(50);
    rightTire.SetPercent(50);
    Sleep(sec);
}

/* This functions makes the robot turn left using time */
void turnLeft(float sec) {
    //left tire speed should be 0 or drive backwards to turn left
    leftTire.SetPercent(-50);
    rightTire.SetPercent(50);
    Sleep(sec);
}

/* This function makes the robot turn right using time */
void turnRight(float sec) {
    //right tire speed should be 0 to turn right
    leftTire.SetPercent(50);
    rightTire.SetPercent(-50);
    Sleep(sec);
}

/* This function makes the robot turn left backward using time */
void turnLeftBackward(float sec) {
    //left tire speed should be 0 to turn left
    leftTire.SetPercent(50);
    rightTire.SetPercent(-50);
    Sleep(sec);
}

/* This function makes the robot turn right backward using time */
void turnRightBackward(float sec) {
    //right tire speed should be 0 to turn right
    leftTire.SetPercent(-50);
    rightTire.SetPercent(50);
    Sleep(sec);
}

/* This function stops the robot from moving */
void stop() {
    leftTire.SetPercent(0);
    rightTire.SetPercent(0);
    Sleep(1.0);
}

/* This function programmed the robot for checkpoint one using time-based */
void checkpoint1() {
    //wait for the start light
    waitForStartLight();
    //move the robot _ seconds
    
    driveForward(1.0);
    //turn left halfway?
    turnLeft(.125);
    //drive forward _ seconds
    driveForward(1.0);
    //turn right 90 degree
    turnRight(.25);
    //drive forward _ seconds (up the ramp)
    driveForward(3.0);
    //turn right slightly
    turnRight(.125);
    //drive forward _second
    driveForward(1.0);
    //turn left slightly
    turnLeft(.125);
    //drive forward _ seconds (until hit the ticket kiosk)
    driveForward(1.0);
    //turn left backward
    turnLeftBackward(.25);
    //turn right forward
    turnRight(.25);
    //drive forward (down the ramp)
    driveForward(2.0);
    //stop
    stop();
}


//shaft encoding methods
//given a distance we will calculate whats needed to make the robot move that much
/* This function drives the robot forward a given distance using shaft encoding */
void moveForwardDistance(float distance)
{
    //calculate the number of revolutions using distance
    float numRevolutions = distance/(WHEEL_Radius*2*PI); 
    //reset counts on the shaft
    leftShaft.ResetCounts();
    rightShaft.ResetCounts();
    //set speed of the tire
    leftTire.SetPercent(50);
    rightTire.SetPercent(50);
    
    //the if statements are there to reduce errors. 2 ways shown. use whichever works. 

    //way 1
    while((rightShaft.Counts() <= TRANSLATIONS_PER_REV * numRevolutions) && (leftShaft.Counts() <= TRANSLATIONS_PER_REV * numRevolutions)){
        if(rightShaft.Counts() == TRANSLATIONS_PER_REV * numRevolutions) {
            //stop the right wheel if its reached the distance
            rightTire.SetPercent(0);
        }
        if(leftShaft.Counts() == TRANSLATIONS_PER_REV * numRevolutions) {
            //stop the left wheel if its reached the distance
            leftTire.SetPercent(0);
        }
    }

    //might need these to stop for tasks
    leftTire.SetPercent(0);
    rightTire.SetPercent(0);
    

    /* way 2 THIS ShOULD ALSO WORK
    for(int i = 0; i < numRevolutions; i++)
    {
        while((rightShaft.Counts() <= TRANSLATIONS_PER_REV) && (leftShaft.Counts() <= TRANSLATIONS_PER_REV)){
        if(rightShaft.Counts() <= TRANSLATIONS_PER_REV * numRevolutions)
            leftTire.SetPercent(50);
        if(leftShaft.Counts() <= TRANSLATIONS_PER_REV * numRevolutions)
            rightTire.SetPercent(50);
        }
    rightShaft.ResetCounts();
    leftShaft.ResetCounts();
    }
    */
    
    //might need these to stop for tasks
    //leftTire.SetPercent(0);
    //rightTire.SetPercent(0);
}

/* This function drives the robot backward a given distance using shaft encoding */
void moveBackwardDistance(float distance)
{
    //calculate the number of revolutions using distance
    float numRevolutions = distance/(WHEEL_Radius*2*PI); 
    //reset counts on the shaft
    leftShaft.ResetCounts();
    rightShaft.ResetCounts();
    //set speed of the tire
    leftTire.SetPercent(-50);
    rightTire.SetPercent(-50);
    
    while((rightShaft.Counts() <= TRANSLATIONS_PER_REV * numRevolutions) && (leftShaft.Counts() <= TRANSLATIONS_PER_REV * numRevolutions)){
        if(rightShaft.Counts() == TRANSLATIONS_PER_REV * numRevolutions) {
            //stop the right wheel if its reached the distance
            rightTire.SetPercent(0);
        }
        if(leftShaft.Counts() == TRANSLATIONS_PER_REV * numRevolutions) {
            //stop the left wheel if its reached the distance
            leftTire.SetPercent(0);
        }
    }

    //might need these to stop for tasks
    leftTire.SetPercent(0);
    rightTire.SetPercent(0);
    
}

/* This function programmed the robot for checkpoint 1 using distance */
void checkpoint1A() {
    //wait for the start light
    waitForStartLight();
    LCD.Write("HIIII");
    //move the robot out the starting position
    moveForwardDistance(20);

    //turn left (halfway) to drive toward the ramp
    turnLeft(.37);

    //move forward to reach the ramp
    moveForwardDistance(23);
    
    //turn right (90 degrees) to face the ramp
    turnRight(.72);

    //drive forward up the ramp 
    moveForwardDistance(45);

    //turn right (90 degree) to face the passport stamp
    turnRight(.65);

    //move forward towards the passport stamp
    moveForwardDistance(25);

    //turn left (90 degree) to face the ticket kiosk
    turnLeft(.5);

    //drive towards the ticket kiosk to touch
    moveForwardDistance(32);

    //drive backward towards the luggage
    moveBackwardDistance(25);

    //turn right backward to face the wall of the robot course
    turnRightBackward(.65);

    //drive to be in front of top of ramp
    moveForwardDistance(20);
    
    //drive backward towards the passport stamp to make space for turning
    moveBackwardDistance(1.5);

    //turn left (90 degree) to face the ramp
    turnLeft(.8);

    //drive down the ramp
    moveForwardDistance(30);

    //stop
    stop();
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
    checkpoint1A();

    return 0;
}