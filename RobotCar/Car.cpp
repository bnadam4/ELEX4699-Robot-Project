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
    // std::string key;
    // bool right_turn = 0, left_turn = 0, for_mov = 0, back_mov = 0, stop_mov = 1;

    while(_com.get_key() != "q")
    {
        _com.communicate();
        /*
        key = _com.get_key();

        if (key == "w") {
        std::cout << "Foward command recieved" << std::endl;
		right_turn = 0;
		left_turn = 0;
		for_mov = 1;
		back_mov = 0;
		stop_mov = 0;
        }
        else if (key == "s") {
        std::cout << "Backwards command recieved" << std::endl;
            right_turn = 0;
            left_turn = 0;
            for_mov = 0;
            back_mov = 1;
            stop_mov = 0;
        }
        else if (key == "d") {
        std::cout << "Right command recieved" << std::endl;
            right_turn = 1;
            left_turn = 0;
            for_mov = 0;
            back_mov = 0;
            stop_mov = 0;
        }
        else if (key == "a") {
        std::cout << "Left command recieved" << std::endl;
            right_turn = 0;
            left_turn = 1;
            for_mov = 0;
            back_mov = 0;
            stop_mov = 0;
        }
        else if (key == "f") {
        std::cout << "Stop command recieved" << std::endl;
            right_turn = 0;
            left_turn = 0;
            for_mov = 0;
            back_mov = 0;
            stop_mov = 1;
        } else
            std::cout << "Got something else: " << key << std::endl;

        if(for_mov)
        {
            _motors.forwards(.5,100);
        }

        if(back_mov)
        {
            _motors.backward(.5,100);
        }

        if(left_turn)
        {
            _motors.left(.5,100);
        }

        if(right_turn)
        {
            _motors.right(.5,100);
        }

        if(stop_mov)
        {
            _motors.stop();
        }
        */
    }
}
