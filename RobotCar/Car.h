#ifndef CAR_H_INCLUDED
#define CAR_H_INCLUDED

#include "Communication.h"
#include "Motor.h"
#include "Guidance.h"
#include <string>


class CCar
{
private:
    CCommunication _com;
    CMotors _motors;
    CGuidance _guide;
public:
    CCar();
    ~CCar();

    // Timing variables for firing
    bool firing;
    double start_time;
    bool automatic;
    bool stop;

    double rotate_time;

    int right_pwm;
    int left_pwm;

    void drive();
    void set_auto(bool automatic);
};

#endif // CAR_H_INCLUDED
