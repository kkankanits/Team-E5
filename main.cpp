#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHMotor.h>


class Movement{ //checkpoint 1
FEHMotor leftTire(FEHMotor::Motor0, 9);
FEHMotor rightTire(FEHMotor::Motor1, 9);
    Movement()
    {
        
    }
    //given a distance we will calculate whats needed to make the robot move that much
    void moveDistance(float distance)
    {
        //PID tutorial
    }
    void toBoardingAndBack() //for checkpoint 1
    {

    }
    void setLeftPercent(float value)
    {
        leftTire.setPercent(value);
    }
    void setRightPercent(float value)
    {
        rightTire.setPercent(value);
    }

};
int main(void)
{
    LCD.Clear(BLACK);
    LCD.WriteLine("Hello, World!");
    LCD.WriteLine("Hello, World!");
    return 0;
}
