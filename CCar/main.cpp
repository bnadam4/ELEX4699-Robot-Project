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
    int turn_camera = 1000;

    while(!closeWindow)
    {
        cin >> key;
         
        //Move forward constantly if w is pressed
        if (key == 'w') {
		      right_turn = 0;
		      left_turn = 0;
		      for_mov = 1;
		      back_mov = 0;
		      stop_mov = 0;
        }

        //Move backwards constantly if s is pressed
        if (key == 's') {
            right_turn = 0;
            left_turn = 0;
            for_mov = 0;
            back_mov = 1;
            stop_mov = 0;
        }

        //Move right constantly if d is pressed
        if (key == 'd') {
            right_turn = 1;
            left_turn = 0;
            for_mov = 0;
            back_mov = 0;
            stop_mov = 0;
        }

        //Move left constantly if a is pressed
        if (key == 'a') {
            right_turn = 0;
            left_turn = 1;
            for_mov = 0;
            back_mov = 0;
            stop_mov = 0;
        }

        //Stop moving if r is pressed
        if (key == 'r') {
            right_turn = 0;
            left_turn = 0;
            for_mov = 0;
            back_mov = 0;
            stop_mov = 1;
        }

        //Exit program
        if (key == 'x') {
            closeWindow = 1;
        }

        //Rotate cannon/camera left incrementally every time q is pressed
        if(key == 'q'){
            turn_camera += 100;
            if (turn_camera >= 2000)
                turn_camera = 2000;
            motor.turn_cannon(turn_camera);
        }

        //Rotate cannon/camera right incrementally every time q is pressed
        if(key == 'e'){
            turn_camera -= 100;
            if (turn_camera <= 600)
                turn_camera = 600;
            motor.turn_cannon(turn_camera);

        }

        //fire the pellets if f is pressed
        if(key == 'f'){
            motor.fire();
        }

        //move forward, speed can be adjusted from 0-255 with 255 being max
        if(for_mov)
        {
            motor.forwards(.5,255);
        }

        //move backwards, speed can be adjusted from 0-255 with 255 being max
        if(back_mov)
        {
            motor.backward(.5,255);
        }

        //move left, speed can be adjusted from 0-255 with 255 being max
        if(left_turn)
        {
            motor.left(.5,255);
        }

        //move right, speed can be adjusted from 0-255 with 255 being max
        if(right_turn)
        {
            motor.right(.5,255);
        }

        //stop both left and right motors
        if(stop_mov)
        {
            motor.stop();
        }


    }




}
