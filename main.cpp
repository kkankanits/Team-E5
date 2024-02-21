#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHMotor.h>


class Movement{ //checkpoint 1
FEHMotor leftTire(FEHMotor::Motor0, 9);
FEHMotor rightTire(FEHMotor::Motor1, 9);
    Movement()
    {
        leftTire.setPercent(50);
        rightTire.setPercent(50);
    }
    //given a distance we will calculate whats needed to make the robot move that much
    void moveDistance(float distance)
    {
        
    }
    void toBoardingAndBack()
    {

    }
};
int main(void)
{
    LCD.Clear(BLACK);
    LCD.WriteLine("Hello, World!");
    LCD.WriteLine("Hello, World!");
    return 0;
}
