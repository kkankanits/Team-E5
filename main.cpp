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
#define ARM_SERVO_MAX 1881
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
        //LCD.Write("Red");
        LCD.Clear(RED);
        LCD.SetFontColor(BLACK);
        LCD.Write(cds.Value());
        return 1;       
    }
    else {
        LCD.Write("Blue");
        LCD.Clear(BLUE);
        LCD.SetFontColor(BLACK);
        LCD.Write(cds.Value());
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

/* This function drives the robot forward until the both of the bump switches are pressed. */
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

/* This function initialize the robot at the start of the run */
void initializeRobot() {
    //setServoMinAndMax
    setMinMaxArmServo();
    setMinMaxRampServo();

    armServo.SetDegree(30);
    rampServo.SetDegree(0);

    //connect to the course
    RCS.InitializeTouchMenu("E5NPDU9yC");

    Sleep(.5);
    //wait for start light
    waitForStartLight();
    //hit the start button
    moveBackward(25, distanceToCount(3.5), 1.3);
}

/* This function drives the robot up the ramp and drop the luggage. */
 void taskSuitcase()
 {
    LCD.Write(RCS.CurrentRegion());
    int course = RCS.CurrentRegion();
    
    //get out of start light area
    moveForward(25, 4.2, 5.0);

    //turn right towards big ramp
    turnRight(50, 115, 3.0);

    //repositioning the robot after got out of the starting area
    moveForward(25, 6, 4);
    turnRight(50, 225, 2);

    //drives forward into the wall to align the robot
    forwardUntilSwitchPressed(25);

    //moves backward to the ramp
    moveBackward(25, 1, 5);

    //turn left to face the ramp
    turnLeft(50, 220, 3);

    //move up the ramp
    moveForward(30, 6, 10);

    if(course == 0) { 
        moveForward(50, 10, 10);
        // adjust the lean right, turn left slightly
        turnRight(50, 15, 5);
        moveForward(50, 12, 10);
    }
    else if(course == 1) {
        moveForward(50, 10, 10);
        // adjust the lean right, turn left slightly
        turnRight(50, 15, 5);
        moveForward(50, 12, 10);
    }
    else if(course == 2) {
        moveForward(50, 10, 10);
        // adjust the lean right, turn left slightly
        //turnLeft(50, 15, 5);
        moveForward(50, 12, 10);
    }
    else {
        //move up the ramp with higher speed
        moveForward(50, 22, 10);
    }



    //turn right 90 degree to face the wall
    turnRight(50, 245, 2.0);

    //hit wall
    forwardUntilSwitchPressed(35);

    //backward halfway to the passport drop
    moveBackward(25, 10, 7);

    //slight right turn to align
    turnRight(50, 15, 3);

    //backward all the way to the passport drop
    moveBackward(25, 9.5, 7);

    //drop luggage 
    rampServo.SetDegree(180);

    //wait to make sure the suitcase is dropped
    Sleep(2.0);

    //move ramp back
    rampServo.SetDegree(100);

    //move forward back to ramp to hit the wall and reposition
    //forwardUntilSwitchPressed(35);
 }

/* This function drives to read the boarding pass light and press the button accordingly. */
 void taskBoardingPass() 
 {
    //left 180
    turnLeft(50, 455, 6);

    //hit the wall
    forwardUntilSwitchPressed(35);

    int backDis = 20.8;

    //if it is course A, back up less
    if(RCS.CurrentRegion() == 0)
    {
        backDis = 20.5;
    }
    //back up
    moveBackward(25, backDis, 10);

    //turn right to the light
    turnRight(50, 125, 3);

    //forward to read light
    //moveForward(25, 15, 5);

    forwardUntilSwitchPressed(35);

    //back to read light
    moveBackward(25, 3.2, 2);

    Sleep(3.0);

    //read light
    bool red = isRedLight();

    //back after reading light
    moveBackward(25, 4.6, 2);

    //turn to align with button
    turnRight(50, 380, 3);

    if(red)
    {
        //go to red
        moveForward(25, 6, 4.);
    }
    else
    {
        //go to blue
        moveForward(25, 1.9, 4.0);
        
    }

    //turn towards button
    turnLeft(50, 220, 4);

    //hit button
    forwardUntilSwitchPressed(35);

    //make sure it hit button
    moveForward(25, 1, 2.);

    

    if(!red)
    {
        //code to hit white button
        moveBackward(30, 4.1, 5);
        //right to the white
        turnRight(50, 220, 4);
        //forward a bit
        moveForward(25, 4.7, 4.);
        //left to face white button
        turnLeft(50, 220, 4);
        //forward to white button
        moveForward(25, .8, 5);

    }
    else {
        //press white button
        moveBackward(25, 3.9, 2.0);
    }

    //press white button
    armServo.SetDegree(125);
    Sleep(.5);
    armServo.SetDegree(30);

    //move out
    moveBackward(30, 15.9, 5);
    if(red) {
        moveBackward(25, 1.1, 5);
    }

    //right 90
    turnRight(50, 220, 3);

    //move until bump switch pressed
    forwardUntilSwitchPressed(35);

 }

/* This function flips the passport lever to stamp the passport. */
 void taskPassport()
 {
    //move back to align with passport
    moveBackward(25, 1.95, 10);

    //turn to face passport
    turnLeft(50, 217, 5);

    //move back to flip passport
    moveBackward(25, 5, 5);

    //move the arm down
    armServo.SetDegree(180);


    //move forward to flip passport
    moveForward(25, 4.3, 5);

    //move the arm up
    armServo.SetDegree(110);

    //Sleep(1.0);

    //move forward to flip passport
    moveForward(30, 4, 3);

    //move the arm up
    armServo.SetDegree(100);

    //move forward to flip passport
    moveForward(30, 4, 3);

    //start of code to flip it back down
    moveBackward(30, 2, 4);

    //arm down
    armServo.SetDegree(150);

    //move forward to rotate lever down
    moveForward(30, 2, 5);

    armServo.SetDegree(90);

    moveBackward(30, 1, 4);

    Sleep(2.0);

    //end of code to bring arm down

    //mov arm back
    //move the arm down
    armServo.SetDegree(180);

    //move backward away from passport
    moveBackward(25, 7, 5);

    //move the arm up
    armServo.SetDegree(30);

    //turn left to face wall
    turnRight(50, 215, 3);

    forwardUntilSwitchPressed(35);

    //then skip fuel lever
 }
 
/* This function drives the robot down the ramp, flips the fuel lever, and press the stop button.  */
 void taskFuelLever() {

    
    //back up before going down ranp
    moveBackward(25, 2, 2);

    //turn towards ramp
    turnRight(25, 240, 3);

    //drive down
    moveForward(25, 19.5, 10);

    //turn toards wall
    turnLeft(50, 210, 4);

    //hit bump
    forwardUntilSwitchPressed(35);

    //backup before turning
    moveBackward(25, 2, 2);
    
    //turn 180 towards the fuel lever
    turnRight(50, 490, 4);

    //reset servo motor
    armServo.SetDegree(40);

    // Get correct lever from the RCS
    //int correctLever = 2;
    int correctLever = RCS.GetCorrectLever();
    LCD.Write(correctLever);
    //A = 4.5
    float distanceToLever;
    float backwardDistance;
     
    // Check which lever to flip and perform some action
    if(correctLever == 0)
    {
        // Perform actions to flip left lever
        distanceToLever = 16.8;
        backwardDistance = 4.5;
    } 
    else if(correctLever == 1)
    {
        // Perform actions to flip middle lever
        distanceToLever = 20;
        backwardDistance = 4.7;
    }
    else if(correctLever == 2)
    {
       // Perform actions to flip right lever
       distanceToLever = 23.5;
       backwardDistance = 4.5;
    }

    // move to the fuel lever
    moveForward(25, distanceToLever, 8.);

    // turn left facing the fuel switches
    turnLeft(50, 217, 3.);

    // back up a little to give space
    moveBackward(25, backwardDistance, 3.);

    float start = TimeNow();
    //timeout if servo doesnt move
    while(TimeNow() - start < 1.0) {
        //flip the lever down
        armServo.SetDegree(150);
    }
    
    //reverse backward 
    moveBackward(25, 2.5, 3.);

    //sleep for 5 sec
    Sleep(7.0);

    //move the servo down a bit
    armServo.SetDegree(170);

    //move up to the lever
    moveForward(25, 3.8, 3.);

    //lift the lever back up
    armServo.SetDegree(105);

    // move up a little to give space
    moveBackward(25, 3.8, 3.);

    //move arm back up
    armServo.SetDegree(40);

    //turn left 90 degree
    turnLeft(50, 217, 3.);

    //move forward however much move forward
    if(correctLever == 0) {
        moveForward(25, 4.8, 10);
    }
    else {
        moveForward(25, distanceToLever-6.5, 10);
    }
    

 }

 void hitStopButton()
 {
    //turnLeft(50, 65, 5);
    //moveForward(25, 4, 10);
    turnRight(50, 70, 5);
    moveForward(40, 40, 10);
    moveBackward(25, 2, 3.);
    turnRight(50, 20, 5);
    moveForward(25, 3, 3.);
    moveBackward(25, 2, 3.);
 }


void finalRun()
{
    initializeRobot();
    taskSuitcase();
    taskBoardingPass();
    taskPassport();
    taskFuelLever();
    hitStopButton();
}

int main(void)
{
    // test comment
    LCD.Clear(BLACK);

    //armServo.TouchCalibrate();

    finalRun();

    //turnRight(50, 235, 5);
    //turnLeft(50, 230, 5);

    return 0;

}