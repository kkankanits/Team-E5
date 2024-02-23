#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHMotor.h>
FEHMotor leftTire(FEHMotor::Motor0, 9);
FEHMotor rightTire(FEHMotor::Motor1, 9);

class Tire{ //checkpoint 1

    int speed;
    Tire()
    {
        speed = 0;
    } 
    void resetSpeed(){
        speed = 0;
    }

    void setHalfSpeed()
    {
        speed = 25;
    }
    void setFullSpeed()
    {
        speed = 50;
    }
    int getSpeed() {
        return speed;
    }
    void setSpeed(int s) {
        speed = s;
    }

};

//given a distance we will calculate whats needed to make the robot move that much
void moveDistance(float distance)
{
    //PID tutorial
}
void toBoardingAndBack() //for checkpoint 1
{

}
int main(void)
{
    LCD.Clear(BLACK);
    LCD.WriteLine("Hello, World!");
    LCD.WriteLine("Hello, World!");
    return 0;
}
