#ifndef CAR_H_INCLUDED
#define CAR_H_INCLUDED

#include "Communication.h"
//#include "Motor.h"

class CCar
{
private:
    CCommunication _com;
    //CMotors _motors;
public:
    CCar();
    ~CCar();

    void drive();
};

#endif // CAR_H_INCLUDED
