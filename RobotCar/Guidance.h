#ifndef GUIDANCE_H_INCLUDED
#define GUIDANCE_H_INCLUDED

#include <opencv2/opencv.hpp>
#include "cvui.h"

#define CANVAS_NAME "Display Image"

class CGuidance
{
private:
    cv::Mat _display_im;
    cv::VideoCapture _cap;
    cv::Point gui_position;
    cv::aruco::Dictionary dictionary;
public:
    CGuidance();
    ~CGuidance();

    void update();
    void get_im(cv::Mat &im);
};

#endif // GUIDANCE_H_INCLUDED
