#ifndef COMMUNICATION_H_INCLUDED
#define COMMUNICATION_H_INCLUDED

#include "server.h"
#include "Motor.h"
#include <thread>

class CCommunication
{
private:
    CServer server;
    CMotors _motors;
    static void server_thread(CServer* server);
    std::string key;
    bool right_turn;
    bool left_turn;
    bool for_mov;
    bool back_mov;
    bool stop_mov;
public:
    std::string get_key() { return key; }
    void communicate();
    CCommunication();
    ~CCommunication();
};

#endif // COMMUNICATION_H_INCLUDED
