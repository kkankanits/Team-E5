#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHMotor.h>
FEHMotor leftTire(FEHMotor::Motor0, 9);
FEHMotor rightTire(FEHMotor::Motor1, 9);
AnalogInputPin cds(FEHIO::P0_0);

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
    rightTire.setPercent(speed);
}
void setLeftSpeed(int speed)
{
    leftTire.setPercent(speed);
}
};

void waitForStartLight() {
    //detect any light (will have to change to red)
    while(cds.value() >= 3.0) {

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
    driveForward(3.0);
    //turn left halfway?
    turnLeft(2.0);
    //drive forward _ seconds
    driveForward(2.0);
    //turn right 90 degree
    turnRight(4.0);
    //drive forward _ seconds (up the ramp)
    driveForward(6.0);
    //turn right slightly
    turnRight(2.0);
    //drive forward _second
    driveForward(2.0);
    //turn left slightly
    turnLeft(2.0);
    //drive forward _ seconds (until hit the ticket kiosk)
    driveForward(4.0);
    //turn left backward
    turnLeftBackward(4.0);
    //turn right forward
    turnRight(4.0);
    //drive forward (down the ramp)
    driveForward(5.0);
    //stop
    stop();

    
}


int main(void)
{
    LCD.Clear(BLACK);
    checkpoint1();

    return 0;
}
