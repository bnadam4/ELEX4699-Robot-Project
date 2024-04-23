#ifndef CAR_H_INCLUDED
#define CAR_H_INCLUDED

#include "Communication.h"

class CCar
{
private:
    CCommunication _com;
public:
    CCar();
    ~CCar();

    void drive();
};

#endif // CAR_H_INCLUDED
