#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHMotor.h>
FEHMotor leftTire(FEHMotor::Motor0, 9);
FEHMotor rightTire(FEHMotor::Motor1, 9);


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


int main(void)
{
    LCD.Clear(BLACK);
    LCD.WriteLine("Hello, World!");
    LCD.WriteLine("Hello, World!");
    
    return 0;
}
