#ifndef MOTOR_INCLUDED
#define MOTOR_INCLUDED

#include "pigpio.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <thread>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <unistd.h>

//#define CVUI_IMPLEMENTATION
//#include "cvui.h"

#define RM_FORWARD 17
#define RM_REVERSE 27
#define RM_EN 22
#define LM_FORWARD 6
#define LM_REVERSE 5
#define LM_EN 26

#define XAXIS 128
#define YAXIS 144
#define PB1 13
#define LED 12
#define SERVO_1 4
#define _CANVAS "exit"

#define SERVO_CANNON 4
#define SERVO_TURN_CANNON 23

#define FIRE_PULL 900
#define FIRE_READY 1800

class CMotors
{
public:
	CMotors();
	~CMotors();

	// Timing variables for firing
	double start_time;
	bool firing;

	void set_pwm_left(int val);
	void set_pwm_right(int val);
	int get_pwm_left();
	int get_pwm_right();

	void stop();
	void forwards(int duty);
	void backward(int duty);

	void left(int duty);
	void right(int duty);

	void turn_cannon(int angle);

	bool fire(double start_time);
};

#endif // MOTOR_INCLUDED
