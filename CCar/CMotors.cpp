#pragma once
////////////////////////////////////////////////////////////////
// ELEX 4618 Lab 1
// Created January 10, 2023 by Braedon Linforth
////////////////////////////////////////////////////////////////
#include "pigpio.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include "CMotors.h"
#include <fstream>

//#define CVUI_IMPLEMENTATION
#include "cvui.h"

#define RM_FORWARD 17
#define RM_REVERSE 27
#define RM_EN 22
#define LM_FORWARD 6
#define LM_REVERSE 5
#define LM_EN 26

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

void CMotors::forwards(float time, int duty)
{

   double start_time = cv::getTickCount();

   while (true)
   {
      double elapsed_time = (cv::getTickCount() - start_time) / cv::getTickFrequency();

      if (elapsed_time >= time)
      {
         break;
      }

      set_pwm_left(duty);
      set_pwm_right(duty);

      gpioWrite(RM_FORWARD, 1);
      gpioWrite(RM_REVERSE, 0);

      gpioWrite(LM_FORWARD, 1);
      gpioWrite(LM_REVERSE, 0);
   }


}

void CMotors::backward(float time, int duty)
{

   double start_time = cv::getTickCount();

   while (true)
   {
      double elapsed_time = (cv::getTickCount() - start_time) / cv::getTickFrequency();

      if (elapsed_time >= time)
      {
         break;
      }

      set_pwm_left(duty);
      set_pwm_right(duty);

      gpioWrite(RM_FORWARD, 0);
      gpioWrite(RM_REVERSE, 1);

      gpioWrite(LM_FORWARD, 0);
      gpioWrite(LM_REVERSE, 1);
   }


}

void CMotors::left(float time, int duty)
{

   double start_time = cv::getTickCount();

   while (true)
   {
      double elapsed_time = (cv::getTickCount() - start_time) / cv::getTickFrequency();

      if (elapsed_time >= time)
      {
         break;
      }

      set_pwm_left(duty);
      set_pwm_right(duty);

      gpioWrite(RM_FORWARD, 1);
      gpioWrite(RM_REVERSE, 0);

      gpioWrite(LM_FORWARD, 0);
      gpioWrite(LM_REVERSE, 0);
   }


}

void CMotors::right(float time, int duty)
{
   double start_time = cv::getTickCount();

   while (true)
   {
      double elapsed_time = (cv::getTickCount() - start_time) / cv::getTickFrequency();

      if (elapsed_time >= time)
      {
         break;
      }

      set_pwm_left(duty);
      set_pwm_right(duty);

      gpioWrite(RM_FORWARD, 0);
      gpioWrite(RM_REVERSE, 0);

      gpioWrite(LM_FORWARD, 1);
      gpioWrite(LM_REVERSE, 0);
   }
}
