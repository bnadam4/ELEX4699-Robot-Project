#include <iostream>
#include "pigpio.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include "CMotors.h"
#include <unistd.h>
#define CVUI_DISABLE_COMPILATION_NOTICES
#define CVUI_IMPLEMENTATION

#define XAXIS 128
#define YAXIS 144
#define PB1 13
#define LED 12
#define SERVO_1 4
#define _CANVAS "exit"
using namespace std;

int main()
{
    CMotors motor;
    bool right_turn = 0,left_turn = 0, for_mov = 0, back_mov = 0, stop_mov = 1, closeWindow = 0;
    char key;

    while(!closeWindow)
    {
        cin >> key;

        if (key == 'w') {
		right_turn = 0;
		left_turn = 0;
		for_mov = 1;
		back_mov = 0;
		stop_mov = 0;
        }

        if (key == 's') {
            right_turn = 0;
            left_turn = 0;
            for_mov = 0;
            back_mov = 1;
            stop_mov = 0;
        }

        if (key == 'd') {
            right_turn = 1;
            left_turn = 0;
            for_mov = 0;
            back_mov = 0;
            stop_mov = 0;
        }

        if (key == 'a') {
            right_turn = 0;
            left_turn = 1;
            for_mov = 0;
            back_mov = 0;
            stop_mov = 0;
        }

        if (key == 'f') {
            right_turn = 0;
            left_turn = 0;
            for_mov = 0;
            back_mov = 0;
            stop_mov = 1;
        }

        if (key == 'q') {
            closeWindow = 1;
        }

        if(for_mov)
        {
            motor.forwards(.5,50);
        }

        if(back_mov)
        {
            motor.backward(.5,50);
        }

        if(left_turn)
        {
            motor.left(.5,50);
        }

        if(right_turn)
        {
            motor.right(.5,50);
        }

        if(stop_mov)
        {
            motor.stop();
        }

    }




}
