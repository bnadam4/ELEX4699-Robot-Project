#include "Communication.h"

CCommunication::CCommunication()
{
    std::thread t(&server_thread, &server);
    t.detach();

    right_turn = 0;
    left_turn = 0;
    for_mov = 0;
    back_mov = 0;
    stop_mov = 1;
}

CCommunication::~CCommunication()
{
    server.stop();
}

void CCommunication::communicate()
{
    std::vector<std::string> cmds;
    server.get_cmd(cmds);
    if(cmds.size() > 0)
    {
        if (cmds.at(0) != "n")
            key = cmds.at(0);

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

void CCommunication::server_thread(CServer* server)
{
    // Start server
    server->start(4618);
}
