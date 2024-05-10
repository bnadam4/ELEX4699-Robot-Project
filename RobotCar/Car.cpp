#include "Car.h"

CCar::CCar()
{
    firing = false;
    start_time = 0.0;
    automatic = false;
    right_pwm = RIGHT_PWM;
    left_pwm = LEFT_PWM;
    rotate_time = 0;
}

CCar::~CCar()
{
    // Nothing for now
}

void CCar::drive()
{
    std::string key;
    bool right_turn = 0, left_turn = 0, for_mov = 0, back_mov = 0, stop_mov = 1;
    int turn_camera = 1000;
    std::string prev_key;
    std::string auto_key;

    // Hard code to automatic for testing purposes. Remove later
    set_auto(true);
    _com.automatic = automatic;
    _guide.automatic = automatic;

    // Debugging interface
    int num;
    std::cout << "Enter the phase to test: ";
    std::cin >> num;
    std::cout << std::endl << std::endl;
    _guide.phase = num;

    while(_com.get_key() != "x" && _guide.phase != 99)
    {
        _com.communicate();
        _guide.ids = _com.ids;
        _guide.distance = _com.dist;
        _guide.pixels_off = _com.pixels_off;
        _guide.update();
        key = _com.get_key();
        auto_key = _guide.auto_key;

        if(_guide.pixels_off.size() > 0)
            std::cout << "pixels off = " << _guide.pixels_off.at(0) << std::endl;

        if( automatic )
        {
            //std::cout << "Got into the automatic block" << std::endl;
            // Do automatic mode
            if(auto_key == "skew forward")
            {
                std::cout << "skew fowards, right_pwm = " << _guide.right_pwm << std::endl;
                _motors.forwards(_guide.right_pwm, LEFT_PWM);
                turn_camera = 2000;
                _motors.turn_cannon(2000);

            }
            else if(auto_key == "forward")
            {
                // Incomplete currently
                std::cout << "auto fowards, right_pwm = " << _guide.right_pwm << std::endl;
                _motors.forwards(_guide.right_pwm, LEFT_PWM);
                turn_camera = CANNON_CENTER;
                _motors.turn_cannon(turn_camera);

            }
            else if(auto_key == "reverse")
            {
                std::cout << "reverse now" << std::endl;
                _motors.backward(_guide.right_pwm, LEFT_PWM);
                turn_camera = CANNON_CENTER;
                _motors.turn_cannon(turn_camera);

            }
            else if(auto_key == "left_reverse")
            {
                std::cout << "left reverse" << std::endl;
                _motors.left_reverse(_guide.right_pwm, LEFT_PWM);
                turn_camera = CANNON_CENTER;
                _motors.turn_cannon(turn_camera);
            }
            else if(auto_key == "rotate left")
            {
                if((cv::getTickCount() - rotate_time)/cv::getTickFrequency() > 0.4)
                {
                    rotate_time = cv::getTickCount();
                    std::cout << "Rotate launcher left" << std::endl;
                    turn_camera += 100;
                    if (turn_camera >= 1800)
                        turn_camera = 1800;
                    _motors.turn_cannon(turn_camera);
                }
            }
            else if(auto_key == "rotate right")
            {
                if((cv::getTickCount() - rotate_time)/cv::getTickFrequency() > 0.4)
                {
                    rotate_time = cv::getTickCount();
                    std::cout << "Rotate launcher right" << std::endl;
                    turn_camera -= 100;
                    if (turn_camera <= 500)
                        turn_camera = 500;
                    _motors.turn_cannon(turn_camera);
                }
            }
            //fire one pellet
            else if(auto_key == "fire")
            {
                    std::cout << "Fire!!!!!" << std::endl;
                    _motors.delay_fire();
                    _motors.delay_fire();
                    _motors.delay_fire();
                    turn_camera = CANNON_CENTER;
                    _motors.turn_cannon(turn_camera);
            }
            //Turn right
            else if(auto_key == "left"){
                turn_camera = CANNON_CENTER;
                _motors.turn_cannon(turn_camera);
                std::cout << "Turn left" << std::endl;
                _motors.right(MIN_PWM, SLOW_TURN);
            }
            //Turn left
            else if(auto_key == "right") {
                turn_camera = CANNON_CENTER;
                _motors.turn_cannon(turn_camera);
                std::cout << "Turn right" << std::endl;
                _motors.left(SLOW_TURN, MIN_PWM);
            }
            //Stop
            else if(auto_key == "stop")
            {
                std::cout << "Stop command recieved" << std::endl;
                _motors.stop();
            }
        }
        else
        {
            // Do manual mode
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
            else if (key == "a") {
                right_turn = 1;
                left_turn = 0;
                for_mov = 0;
                back_mov = 0;
                stop_mov = 0;
            }
            else if (key == "d") {
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
            //Rotate cannon/camera right incrementally every time e is pressed
            else if(key == "e"){
                turn_camera -= 100;
                if (turn_camera <= 600)
                    turn_camera = 600;
                _motors.turn_cannon(turn_camera);

            }
            else if(key == "i")
            {
                turn_camera = CANNON_CENTER;
                _motors.turn_cannon(turn_camera);
            }
            else if(key == "u" && key != prev_key)
            {
                right_pwm += 10;
            }
            else if(key == "o" && key != prev_key)
            {
                right_pwm -= 10;
            }
            }



        //fire the pellets if f is pressed
        if(key == "f" ){
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
                std::cout << "Rotate left" << std::endl;
            else if(key == "e")
                std::cout << "Rotate right" << std::endl;
            else
                std::cout << "Got something else: " << key << std::endl;
        }

        prev_key = key;

        if(for_mov)
            {
                _motors.forwards(RIGHT_PWM, LEFT_PWM);
            }

            if(back_mov)
            {
                _motors.backward(RIGHT_PWM, LEFT_PWM);
            }

            if(left_turn)
            {
                _motors.left(RIGHT_PWM, LEFT_PWM);
            }

            if(right_turn)
            {
                _motors.right(RIGHT_PWM, LEFT_PWM);
            }

            if(stop_mov)
            {
                _motors.stop();
            }
        }
    }

    _motors.stop();

    _motors.turn_cannon(CANNON_CENTER);
}

void CCar::set_auto(bool aut)
{
    automatic = aut;
    _com.automatic = aut;
}
