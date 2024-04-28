#ifndef GUIDANCE_H_INCLUDED
#define GUIDANCE_H_INCLUDED

#include <opencv2/opencv.hpp>
#include "server.h"
#include "cvui.h"

#define CANVAS_NAME "Display Image"

class CGuidance
{
private:
    //CServer image_server;
    //static void image_thread(CServer* img_server);
public:
    CGuidance();
    ~CGuidance();

    void update();
    //void get_im(cv::Mat &im);
};

#endif // GUIDANCE_H_INCLUDED
