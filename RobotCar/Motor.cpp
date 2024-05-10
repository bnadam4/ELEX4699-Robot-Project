////////////////////////////////////////////////////////////////
// ELEX 4618 Lab 1
// Created January 10, 2023 by Braedon Linforth
////////////////////////////////////////////////////////////////

#include "Motor.h"

CMotors::CMotors()
{

   //Allows GPIOs to be modified
   gpioInitialise();

   // Right Motor Initializing
   gpioSetMode(RM_FORWARD, PI_OUTPUT);
   gpioSetMode(RM_REVERSE, PI_OUTPUT);
   gpioSetMode(RM_EN, PI_OUTPUT);
   gpioSetPWMrange(RM_EN, 255);

   // Left Motor Initializing
   gpioSetMode(LM_FORWARD, PI_OUTPUT);
   gpioSetMode(LM_REVERSE, PI_OUTPUT);
   gpioSetMode(LM_EN, PI_OUTPUT);
   gpioSetPWMrange(RM_EN, 255);

   // Camera / Cannon servo Initilizing
   gpioSetMode(SERVO_CANNON, PI_OUTPUT);
   gpioSetMode(SERVO_TURN_CANNON, PI_OUTPUT);
}

CMotors::~CMotors()
{
    //kills all GPIOs, must be terminated else GPIOs get locked
    gpioTerminate();
}


void CMotors::set_pwm_left(int val)
{
   gpioPWM(LM_EN, val);
}

void CMotors::set_pwm_right(int val)
{
   gpioPWM(RM_EN, val);
}

void CMotors::stop()
{
      set_pwm_left(0);
      set_pwm_right(0);

      gpioWrite(RM_FORWARD, 0);
      gpioWrite(RM_REVERSE, 0);

      gpioWrite(LM_FORWARD, 0);
      gpioWrite(LM_REVERSE, 0);
}

void CMotors::forwards(int right_duty, int left_duty)
{
      set_pwm_left(left_duty);
      set_pwm_right(right_duty);

      gpioWrite(RM_FORWARD, 1);
      gpioWrite(RM_REVERSE, 0);

      gpioWrite(LM_FORWARD, 1);
      gpioWrite(LM_REVERSE, 0);
}

void CMotors::backward(int right_duty, int left_duty)
{
    set_pwm_left(left_duty);
    set_pwm_right(right_duty);

    gpioWrite(RM_FORWARD, 0);
    gpioWrite(RM_REVERSE, 1);

    gpioWrite(LM_FORWARD, 0);
    gpioWrite(LM_REVERSE, 1);
}

void CMotors::left(int right_duty, int left_duty)
{
      set_pwm_left(left_duty);
      set_pwm_right(right_duty);

      gpioWrite(RM_FORWARD, 1);
      gpioWrite(RM_REVERSE, 0);

      gpioWrite(LM_FORWARD, 0);
      gpioWrite(LM_REVERSE, 0);
}

void CMotors::right(int right_duty, int left_duty)
{
    set_pwm_left(left_duty);
    set_pwm_right(right_duty);

    gpioWrite(RM_FORWARD, 0);
    gpioWrite(RM_REVERSE, 0);

    gpioWrite(LM_FORWARD, 1);
    gpioWrite(LM_REVERSE, 0);
}

void CMotors::left_reverse(int right_duty, int left_duty)
{
    set_pwm_left(left_duty);
    set_pwm_right(right_duty);

    gpioWrite(RM_FORWARD, 0);
    gpioWrite(RM_REVERSE, 1);

    gpioWrite(LM_FORWARD, 0);
    gpioWrite(LM_REVERSE, 0);
}

void CMotors::turn_cannon(int angle)
{

    gpioServo(SERVO_TURN_CANNON, angle);

}

bool CMotors::fire(double start_time)
{
    //delays for firing and resetting the firing mechanism, may be modified but these seem like optimal values when I was testing
    double delay = 0.25;
    double delay2 = 0.5;

    double elapsed_time = (cv::getTickCount() - start_time) / cv::getTickFrequency();

    if (elapsed_time < delay)
    {
        gpioServo(SERVO_CANNON, FIRE_PULL );
        return true;
    }
    else if (elapsed_time >= delay && elapsed_time < delay2)
    {
        gpioServo(SERVO_CANNON, FIRE_READY );
        return true;
    }
    else if(elapsed_time >= delay2)
    {
        return false;
    }
    else{
        // Error case
        return false;
    }
}

void CMotors::delay_fire()
{
    //delays for firing and resetting the firing mechanism, may be modified but these seem like optimal values when I was testing
    double start_time = cv::getTickCount();
    double delay = 0.25;
    double delay2 = 0.5;

    gpioServo(SERVO_CANNON, FIRE_PULL );

    while (true)
   {
      double elapsed_time = (cv::getTickCount() - start_time) / cv::getTickFrequency();

      if (elapsed_time >= delay)
      {
         gpioServo(SERVO_CANNON, FIRE_READY );

      }

      if(elapsed_time >= delay2)
      {
        break;
      }
   }
}

void CMotors::degree_turn(int degrees)
{

}
