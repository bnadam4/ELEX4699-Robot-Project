////////////////////////////////////////////////////////////////
// ELEX 4618 Lab 1
// Created January 10, 2023 by Braedon Linforth
////////////////////////////////////////////////////////////////

#include "Motor.h"

CMotors::CMotors()
{

   // Motor Initilizing
   gpioInitialise();
   gpioSetMode(RM_FORWARD, PI_OUTPUT);
   gpioSetMode(RM_REVERSE, PI_OUTPUT);
   gpioSetMode(RM_EN, PI_OUTPUT);
   gpioSetPWMrange(RM_EN, 255);

   gpioSetMode(LM_FORWARD, PI_OUTPUT);
   gpioSetMode(LM_REVERSE, PI_OUTPUT);
   gpioSetMode(LM_EN, PI_OUTPUT);
   gpioSetPWMrange(RM_EN, 255);

}

CMotors::~CMotors()
{
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

void CMotors::forwards(int duty)
{
      set_pwm_left(duty);
      set_pwm_right(duty);

      gpioWrite(RM_FORWARD, 1);
      gpioWrite(RM_REVERSE, 0);

      gpioWrite(LM_FORWARD, 1);
      gpioWrite(LM_REVERSE, 0);
}

void CMotors::backward(int duty)
{
    set_pwm_left(duty);
    set_pwm_right(duty);

    gpioWrite(RM_FORWARD, 0);
    gpioWrite(RM_REVERSE, 1);

    gpioWrite(LM_FORWARD, 0);
    gpioWrite(LM_REVERSE, 1);
}

void CMotors::left(int duty)
{
      set_pwm_left(duty);
      set_pwm_right(duty);

      gpioWrite(RM_FORWARD, 1);
      gpioWrite(RM_REVERSE, 0);

      gpioWrite(LM_FORWARD, 0);
      gpioWrite(LM_REVERSE, 0);
}

void CMotors::right(int duty)
{
    set_pwm_left(duty);
    set_pwm_right(duty);

    gpioWrite(RM_FORWARD, 0);
    gpioWrite(RM_REVERSE, 0);

    gpioWrite(LM_FORWARD, 1);
    gpioWrite(LM_REVERSE, 0);
}
