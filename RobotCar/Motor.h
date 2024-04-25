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

//#define CVUI_IMPLEMENTATION
//#include "cvui.h"

#define RM_FORWARD 17
#define RM_REVERSE 27
#define RM_EN 22
#define LM_FORWARD 6
#define LM_REVERSE 5
#define LM_EN 26

class CMotors
{
public:
	CMotors();
	~CMotors();

	void set_pwm_left(int val);
	void set_pwm_right(int val);
	int get_pwm_left();
	int get_pwm_right();

	void stop();
	void forwards(int duty);
	void backward(int duty);

	void left(int duty);
	void right(int duty);
};

#endif // MOTOR_INCLUDED
