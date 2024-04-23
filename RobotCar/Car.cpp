#include "Car.h"

CCar::CCar()
{
    //nothing for now
}

CCar::~CCar()
{
    //nothing for now
}

void CCar::drive()
{
    while(_com.get_cmd() != "q")
    {
        _com.communicate();
    }
}
