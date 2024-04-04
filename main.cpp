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
#define ARM_SERVO_MIN 500
#define ARM_SERVO_MAX 2270
#define RAMP_SERVO_MIN 581
#define RAMP_SERVO_MAX 2301

//Declaration for CdS cell
AnalogInputPin cds(FEHIO::P1_0);
//Declarations for encoders & motors
DigitalEncoder rightEncoder(FEHIO::P0_0);
DigitalEncoder leftEncoder(FEHIO::P0_1);
FEHMotor rightMotor(FEHMotor::Motor3,9.0);
FEHMotor leftMotor(FEHMotor::Motor1,9.0);
//Declaration for servo motor
FEHServo armServo(FEHServo::Servo7);

//declaration for luggage mechanism;
FEHServo rampServo(FEHServo::Servo0);

/* This function waits for the red start light to trigger the robot to start */
void waitForStartLight() {
    Sleep(0.5);
    //detect any light (will have to change to red)
    while(cds.Value() >= 2.0);
}

int distanceToCount(int distance){
    return (distance*TRANSLATIONS_PER_REV)/(2*M_PI*WHEEL_Radius);
}

void moveForward(float percent, int counts, float timeout) //using encoders 
{
    percent = (BATTERY_FULL / Battery.Voltage()) * percent;
    //Reset encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    //Set both motors to desired percent
    rightMotor.SetPercent(percent);
    leftMotor.SetPercent(percent);

    float start = TimeNow();

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((((leftEncoder.Counts() + rightEncoder.Counts()) / 2.) < counts) && (TimeNow() - start < timeout));

    //Turn off motors
    rightMotor.Stop();
    leftMotor.Stop();
}

void moveBackward(float percent, int counts, float timeout)
{
    moveForward(-1*percent, counts, timeout);
}

void turnRight(float percent, int counts, float timeout) //using encoders 50, 250 for 90 degree
{
    percent = (BATTERY_FULL / Battery.Voltage()) * percent;
    //Reset encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    //set right motor backwards, left motor forwards
    rightMotor.SetPercent(-1*percent);
    leftMotor.SetPercent(percent);

    float start = TimeNow();

    //While the average of the left and right encoder is less than counts, keep running motors
    while(((leftEncoder.Counts() + rightEncoder.Counts()) / 2. < counts) && (TimeNow() - start < timeout));

    //Turn off motors
    rightMotor.Stop();
    leftMotor.Stop();

}

void turnLeft(float percent, int counts, float timeout) //using encoders 50, 250 for 90 degrees
{
    percent = (BATTERY_FULL / Battery.Voltage()) * percent;
    //Reset encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    //set right motor backwards, left motor forwards
    rightMotor.SetPercent(percent);
    leftMotor.SetPercent(-1*percent);

    float start = TimeNow();

    //While the average of the left and right encoder is less than counts, keep running motors
    while(((leftEncoder.Counts() + rightEncoder.Counts()) / 2. < counts) && (TimeNow() - start < timeout));

    //Turn off motors
    rightMotor.Stop();
    leftMotor.Stop();

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

void setMinMaxArmServo() {
    armServo.SetMin(ARM_SERVO_MIN);
    armServo.SetMax(ARM_SERVO_MAX);
}

void setMinMaxRampServo() {
    rampServo.SetMin(RAMP_SERVO_MIN);
    rampServo.SetMax(RAMP_SERVO_MAX);
}

 void checkpoint5()
 {
    //setServoMinAndMax
    setMinMaxArmServo();
    setMinMaxRampServo();

    //reset servo motor
    armServo.SetDegree(40);
    rampServo.SetDegree(0);

    // Initialize the RCS
    //RCS.InitializeTouchMenu("E5NPDU9yC");

    //Sleep(.5);
    //wait for start light
    //waitForStartLight();


    //hit the start button
    moveBackward(25, distanceToCount(3), 1.3);

    // get out of start light area
    moveForward(25, distanceToCount(20), 5.);
    
    // turn left toward the fuel switches
    turnLeft(50, 95, 2.0);

    // forward towards the left steeper ramp
    moveForward(25, distanceToCount(9), 15.);

    // turn right to face ramp
    turnRight(50, 240, 5);

    //line up with the ramp
    moveForward(25, distanceToCount(2), 20.0);

    // get up ramp 
    moveForward(50, distanceToCount(14), 20.0);

    //turn rught slightly on the ramp bc it steers to the left
    turnRight(50, 35, 3.0);

    // get up ramp completely
    moveForward(50, distanceToCount(7), 20.0);

    turnRight(50, 245, 10);

    //move forward to align with luggage
    moveForward(25, distanceToCount(7), 10);

    Sleep(2.0);

    //drop luggage 
    rampServo.SetDegree(180);

    Sleep(2.0);

    /*
    //move forward towards next ramp
    moveForward(25, distanceToCount(20), 10);

    //turn right to face ramp
    turnRight(50, 230, 10.0);

    //go down ramp
    moveForward(25, 15.5, 15.0);

    //turn left to face button
    turnLeft(50, 230, 10.0);

    //hit end button
    moveForward(25, 3.5, 10);
    */
 }


int main(void)
{
    // test comment
    LCD.Clear(BLACK);

    //code for checkpoint 3
    checkpoint5();

    return 0;

}