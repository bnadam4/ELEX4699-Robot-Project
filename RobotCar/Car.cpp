#include "Car.h"

CCar::CCar()
{
    firing = false;
    start_time = 0.0;
}

CCar::~CCar()
{
    //nothing for now
}

void CCar::drive()
{
    std::string key;
    bool right_turn = 0, left_turn = 0, for_mov = 0, back_mov = 0, stop_mov = 1;
    int turn_camera = 1000;
    std::string prev_key;

    while(_com.get_key() != "x")
    {
        _com.communicate();
        _guide.update();
        key = _com.get_key();

        if (_com.get_receiving())
        {
            if (key == "w") {
            right_turn = 0;
            left_turn = 0;
            for_mov = 1;
            back_mov = 0;
            stop_mov = 0;
            }
            else if (key == "s") {
                right_turn = 0;
                left_turn = 0;
                for_mov = 0;
                back_mov = 1;
                stop_mov = 0;
            }
            else if (key == "d") {
                right_turn = 1;
                left_turn = 0;
                for_mov = 0;
                back_mov = 0;
                stop_mov = 0;
            }
            else if (key == "a") {
                right_turn = 0;
                left_turn = 1;
                for_mov = 0;
                back_mov = 0;
                stop_mov = 0;
            }
            //Stop moving if r is press
            else if (key == "r") {
                right_turn = 0;
                left_turn = 0;
                for_mov = 0;
                back_mov = 0;
                stop_mov = 1;
            }
            //Rotate cannon/camera left incrementally every time q is pressed
            else if(key == "q"){
                turn_camera += 100;
                if (turn_camera >= 2000)
                    turn_camera = 2000;
                _motors.turn_cannon(turn_camera);
            }
            //Rotate cannon/camera right incrementally every time q is pressed
            else if(key == "e"){
                turn_camera -= 100;
                if (turn_camera <= 600)
                    turn_camera = 600;
                _motors.turn_cannon(turn_camera);

            }
        }

        //fire the pellets if f is pressed
        if(key == "f"){
            //std::cout << "Entered firing block" << std::endl;
            if(firing == false)
            {
                std::cout << std::endl << "Fire! : " << key << std::endl;
                start_time = cv::getTickCount();
                firing = true;
            }
            firing = _motors.fire(start_time);
        }

        // Debugging outputs
        if (key != prev_key)
        {
            if(key == "w")
                std::cout << "Foward command recieved" << std::endl;
            else if(key == "s")
                std::cout << "Backwards command recieved" << std::endl;
            else if(key == "a")
                std::cout << "Left command recieved" << std::endl;
            else if(key == "d")
                std::cout << "Right command recieved" << std::endl;
            else if(key == "r")
                std::cout << "Stop command recieved" << std::endl;
            else if(key == "q")
                std::cout << "Rotate left" << std::endl;
            else if(key == "e")
                std::cout << "Rotate right" << std::endl;
            else
                std::cout << "Got something else: " << key << std::endl;
        }

        prev_key = key;

        if(for_mov)
            {
                _motors.forwards(200);
            }

            if(back_mov)
            {
                _motors.backward(200);
            }

            if(left_turn)
            {
                _motors.left(200);
            }

            if(right_turn)
            {
                _motors.right(200);
            }

            if(stop_mov)
            {
                _motors.stop();
            }
    }
}
