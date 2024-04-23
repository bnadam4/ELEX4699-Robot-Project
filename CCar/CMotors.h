#pragma once
#include "pigpio.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <string>
#include <sstream>


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
	void forwards(float time, int duty);
	void backward(float time, int duty);

	void left(float time, int duty);
	void right(float time, int duty);
};
