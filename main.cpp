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
#define LINE_SLOPE -0.0405
#define LINE_INTERCEPT 2.1577

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

//declares bumpswithches
DigitalInputPin leftBumpSwitch(FEHIO::P1_1);
DigitalInputPin rightBumpSwitch(FEHIO::P1_2);

/* This function waits for the red start light to trigger the robot to start */
void waitForStartLight() {
    Sleep(0.5);
    //detect any light (will have to change to red)
    while(cds.Value() >= 2.0);
}

/* This function recalculate the distance to counter the differences in motor power */
float correctDistance(float distance, float power) {
    return distance;
}

/* This function calculate the rotational counts using the distance */
int distanceToCount(int distance){
    return (distance*TRANSLATIONS_PER_REV)/(2*M_PI*WHEEL_Radius);
}

/* This function drive the robot forward specific distance using shaft encoding */
void moveForward(float percent, float distance, float timeout)
{
    percent = (BATTERY_FULL / Battery.Voltage()) * percent;
    distance = correctDistance(distance, percent);
    int counts = distanceToCount(distance);

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

/* This function drive the robot backward specific distance using shaft encoding */
void moveBackward(float percent, float distance, float timeout)
{
    moveForward(-1*percent, distance, timeout);
}

/* This function turns the robot to the right depending on the rotational count */
void turnRight(float percent, int counts, float timeout) //using encoders 50, 130 for 90 degree
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
    while(((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < counts)  && (TimeNow() - start < timeout));

    //Turn off motors
    rightMotor.Stop();
    leftMotor.Stop();

}

/* This function turns the robot to the left depending on the rotational count */
void turnLeft(float percent, int counts, float timeout) //using encoders 50, 105 for 90 degrees
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
    while(((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < counts) && (TimeNow() - start < timeout));

    //Turn off motors
    rightMotor.Stop();
    leftMotor.Stop();

}

/* This function check if the light is a red light. */
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

/* This function set the min and max values for the arm servo motor */
void setMinMaxArmServo() {
    armServo.SetMin(ARM_SERVO_MIN);
    armServo.SetMax(ARM_SERVO_MAX);
}

/* This function set the min and max values for the ramp servo motor */
void setMinMaxRampServo() {
    rampServo.SetMin(RAMP_SERVO_MIN);
    rampServo.SetMax(RAMP_SERVO_MAX);
}

void forwardUntilSwitchPressed(int percent)
{
    percent = (BATTERY_FULL / Battery.Voltage()) * percent;


    while(leftBumpSwitch.Value() || rightBumpSwitch.Value())
    {
        
        if(!leftBumpSwitch.Value())
        {
            leftMotor.Stop();
            rightMotor.SetPercent(percent);
            
        }
        
        else if(!rightBumpSwitch.Value())
        {
            rightMotor.Stop();
            leftMotor.SetPercent(percent);

        }
        else{
            rightMotor.SetPercent(percent);
            leftMotor.SetPercent(percent);
        }

    }
    
    moveForward(percent, 1, 0.8);
    rightMotor.Stop();
    leftMotor.Stop();
}

 void taskSuitcase()
 {
    
    // get out of start light area
    moveForward(25, 2, 5.0);

    //turn right towards big ramp
    turnRight(50, 65, 3.0);

    //move to the ramp
    moveForward(25, 3, 4);
    turnRight(50, 130, 2);

    forwardUntilSwitchPressed(25);

    moveBackward(25, 1, 5);

    turnLeft(50, 107, 3);

    //move up the ramp
    moveForward(30, 3, 10);

    // adjust the lean right, turn left slightly
    //turnRight(50, 5, 5);

    //move up the ramp
    moveForward(50, 11.8, 10);

    //turn right 90 degree
    turnRight(50, 145, 2.0);

    //hit wall
    forwardUntilSwitchPressed(35);

    //backward to the passport drop
    moveBackward(25, 5, 7);

    //align
    turnRight(50, 5, 3);

    //backward to the passport drop
    moveBackward(25, 4.8, 7);

    //drop luggage 
    rampServo.SetDegree(180);

    Sleep(2.0);

    // move forward back to ramp
    forwardUntilSwitchPressed(35);
 }

//after suitcase
 void taskPassport()
 {
    //move back to align with passport
    moveBackward(25, 4.7, 5);

    //turn to face passport
    turnRight(50, 110, 5);

    //move back to flip passport
    moveBackward(45, 12.5, 5);

    //move back to flip it back
    moveForward(30, 7.5, 5);
 }


void finalRun()
{
    //setServoMinAndMax
    setMinMaxArmServo();
    setMinMaxRampServo();

    //reset servo motor
    armServo.SetDegree(50);
    rampServo.SetDegree(0);

    // Initialize the RCS
    RCS.InitializeTouchMenu("E5NPDU9yC");

    Sleep(.5);
    //wait for start light
    waitForStartLight();

    //hit the start button
    moveBackward(25, distanceToCount(3.5), 1.3);

    taskSuitcase();
    taskPassport();
}

int main(void)
{
    // test comment
    LCD.Clear(BLACK);

    finalRun();

    //turnRight(50, 130, 5);
    //turnLeft(50, 105, 5);

    return 0;

}