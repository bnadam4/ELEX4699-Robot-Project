#include "Communication.h"

CCommunication::CCommunication()
{
    std::thread t(&server_thread, &server);
    t.detach();
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
        full_cmd = cmds.at(0);
        if(full_cmd == "w")
            std::cout << "Foward" << std::endl;
        else if(full_cmd == "a")
            std::cout << "Left" << std::endl;
        else if(full_cmd == "d")
            std::cout << "Right" << std::endl;
        else if(full_cmd == "s")
            std::cout << "Backwards" << std::endl;
        else if(full_cmd == "q")
            std::cout << "quit" << std::endl;
        else
            std::cout << "Got something else: " << cmds.at(0) << std::endl;
    }
}

void CCommunication::server_thread(CServer* server)
{
    // Start server
    server->start(4618);
}
