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

//declaration for luggage mechanism;
FEHServo luggage(FEHServo::Servo6);

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

void moveBackward(float percent, int counts, float timeout)
{
    moveForward(-1*percent, counts, timeout);
}

void turnRight(float percent, int counts, float timeout) //using encoders 50, 250 for 90 degree
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

void turnLeft(float percent, int counts, float timeout) //using encoders 50, 250 for 90 degrees
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

 void checkpoint5()
 {
    //wait for start light
    waitForStartLight();

    //setServoMinAndMax

    //hit the start button
    moveBackward(25, distanceToCount(3.5), 1.3);

    // get out of start light area
    moveForward(25, distanceToCount(18), 5.);
    
    // turn left toward the fuel switches
    turnLeft(50, 105, 2.);

    // forward towards the left steeper ramp
    moveForward(25, distanceToCount(11), 15.);

    // turn right to face ramp
    turnRight(50, 230, 2.0);

    //line up with the ramp
    moveForward(25, distanceToCount(2), 20.);

    // get up ramp 
    moveForward(40, distanceToCount(10), 20.);

    //turn left slightly on the ramp bc it steer to the right
    turnRight(50, 15, 3.0);

    // get up ramp completely
    moveForward(40, distanceToCount(15), 20.);

    turnRight(50, 230, 10);

    //move forward to align with luggage
    moveForward(25, distanceToCount(10), 10);

    //drop luggage

    //move forward towards next ramp
    moveForward(25, distanceToCount(20), 10);

    //turn right to face ramp
    turnRight(50, 230, 10);

    //go down ramp
    moveForward(25, 15.5, 15);

    //turn left to face button
    turnLeft(50, 230, 10);

    //hit end button
    moveForward(25, 3.5, 10);


 }


int main(void)
{
    // test comment
    LCD.Clear(BLACK);

    //code for checkpoint 3
    checkpoint5();

    return 0;

}