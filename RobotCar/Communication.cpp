#include "Communication.h"

CCommunication::CCommunication()
{
    std::thread t(&server_thread, &server);
    t.detach();

    receiving = false;
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
        receiving = true;
        if (cmds.at(0) != "n")
            key = cmds.at(0);
    } else {
        receiving = false;}
}

void CCommunication::server_thread(CServer* server)
{
    // Start server
    server->start(4618);
}
