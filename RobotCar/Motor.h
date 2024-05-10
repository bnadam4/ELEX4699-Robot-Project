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

#define RM_FORWARD 6
#define RM_REVERSE 5
#define RM_EN 22
#define LM_FORWARD 17
#define LM_REVERSE 27
#define LM_EN 26

#define XAXIS 128
#define YAXIS 144
#define PB1 13
#define LED 12
#define SERVO_1 4
#define _CANVAS "exit"

#define SERVO_CANNON 4
#define SERVO_TURN_CANNON 23
#define CANNON_CENTER 750

#define FIRE_PULL 900
#define FIRE_READY 1800

#define RIGHT_PWM 220
#define LEFT_PWM 210

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
	void forwards(int right_duty, int left_duty);
	void backward(int right_duty, int left_duty);

	void left_reverse(int right_duty, int left_duty);

	void left(int right_duty, int left_duty);
	void right(int right_duty, int left_duty);



	void turn_cannon(int angle);

	bool fire(double start_time);

	void delay_fire();

	void degree_turn(int degrees);
};

#endif // MOTOR_INCLUDED
