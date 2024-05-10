#ifndef COMMUNICATION_H_INCLUDED
#define COMMUNICATION_H_INCLUDED

#include "server.h"
#include <thread>
#include <cmath>

class CCommunication
{
private:
    CServer server;
    void server_thread(CServer* server);
    std::string key;
    bool receiving;
    cv::Mat _display_im;
    cv::VideoCapture _cap;
    cv::Point gui_position;
    cv::aruco::Dictionary dictionary;
    std::vector<std::string> cmds;
    std::vector<std::string> img_cmds;

    // Additions from Alex code
    CServer img_server;
    // std::string gstreamer_pipeline(int capture_width, int capture_height, int framerate, int display_width, int display_height);
    void img_server_thread(CServer* server);
public:
    std::string get_key() { return key; }
    bool get_receiving() { return receiving; }
    int off_center(cv::Point minPoint, cv::Point maxPoint, cv::Mat image);
    void communicate();

    bool automatic;

    std::vector<int> ids;
    std::vector<int> pixels_off;
    std::vector<double> dist;
    CCommunication();
    ~CCommunication();
};

#endif // COMMUNICATION_H_INCLUDED
