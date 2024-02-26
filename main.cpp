#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHMotor.h>

#define WHEEL_Radius 2.5
#define TRANSLATIONS_PER_REV 318
#define PI 3.14

FEHMotor leftTire(FEHMotor::Motor0, 9);
FEHMotor rightTire(FEHMotor::Motor1, 9);
AnalogInputPin cds(FEHIO::P0_0);
DigitalEncoder rightShaft(FEHIO:P0_1);
DigitalEncoder leftShaft(FEHIO:P0_2);

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
void toBoardingAndBack() //for checkpoint 1
{
    
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

void waitForStartLight() {
    //detect any light (will have to change to red)
    while(cds.Value() >= 3.0) {

    }
}

void driveForward(float sec) {
    leftTire.SetPercent(50);
    rightTire.SetPercent(50);
    Sleep(sec);
}

void turnLeft(float sec) {
    //left tire speed should be 0 to turn left
    leftTire.SetPercent(0);
    rightTire.SetPercent(50);
    Sleep(sec);
}

void turnRight(float sec) {
    //right tire speed should be 0 to turn right
    leftTire.SetPercent(50);
    rightTire.SetPercent(0);
    Sleep(sec);
}

void turnLeftBackward(float sec) {
    //left tire speed should be 0 to turn left
    leftTire.SetPercent(0);
    rightTire.SetPercent(-50);
    Sleep(sec);
}

void stop() {
    leftTire.SetPercent(0);
    rightTire.SetPercent(0);
    Sleep(1.0);
}

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
void moveDistance(float distance)
{
    float multiple = distance/(WHEEL_Radius*2*PI);
    leftShaft.resetCounts();
    rightShaft.resetCounts();
    leftTire.SetPercent(50);
    rightTire.SetPercent(50);
    while((rightShaft.Counts() <= TRANSLATIONS_PER_REV * multiple) && (leftShaft.Counts() <= TRANSLATIONS_PER_REV * multiple)){}
    leftTire.SetPercent(0);
    rightTire.SetPercent(0);
}


int main(void)
{
    LCD.Clear(BLACK);
    checkpoint1();

    return 0;
}
