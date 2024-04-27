#ifndef COMMUNICATION_H_INCLUDED
#define COMMUNICATION_H_INCLUDED

#include "server.h"
#include <thread>

class CCommunication
{
private:
    CServer server;
    static void server_thread(CServer* server);
    std::string key;
    bool receiving;
public:
    std::string get_key() { return key; }
    bool get_receiving() { return receiving; }
    void communicate();
    CCommunication();
    ~CCommunication();
};

#endif // COMMUNICATION_H_INCLUDED
