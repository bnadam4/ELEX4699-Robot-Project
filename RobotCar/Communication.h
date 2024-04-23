#ifndef COMMUNICATION_H_INCLUDED
#define COMMUNICATION_H_INCLUDED

#include "server.h"
#include <thread>

class CCommunication
{
private:
    CServer server;
    static void server_thread(CServer* server);
    std::string full_cmd;
public:
    std::string get_cmd() { return full_cmd; }
    void communicate();
    CCommunication();
    ~CCommunication();
};

#endif // COMMUNICATION_H_INCLUDED
