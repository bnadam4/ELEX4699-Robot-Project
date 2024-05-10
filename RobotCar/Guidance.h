#ifndef GUIDANCE_H_INCLUDED
#define GUIDANCE_H_INCLUDED

#include <opencv2/opencv.hpp>
#include "server.h"
#include <string>
#include <cmath>

#define START_MARKER 30
#define TARGET_1 21
#define TARGET_2 22
#define GUIDE_MARKER_1 31
#define TARGET_3 27
#define GUIDE_MARKER_2 32
#define TARGET_4 23
#define END_MARKER 29
#define OBSTACTLE_1 26

#define TOLERANCE 20

#define PWM_LEFT 210
#define PWM_RIGHT 220

#define SLOW_TURN 160
#define MIN_PWM 100

#define TURN_DELAY 0.5
#define AIM_DELAY 0.5

class CGuidance
{
private:
    double elapsed_time;

public:
    std::vector<int> ids;
    std::vector<double> distance;
    std::vector<int> pixels_off;
    bool firing;

    // Variables that determine what is done
    int target_ID;
    std::string auto_key;
    int phase;

    int right_pwm;

    CGuidance();
    ~CGuidance();

    void update();
    void keep_center(int ID_index);

    void select_action();

    bool automatic;
};

#endif // GUIDANCE_H_INCLUDED
