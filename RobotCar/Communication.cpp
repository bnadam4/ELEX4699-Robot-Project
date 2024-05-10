#include "Communication.h"

#define CVUI_DISABLE_COMPILATION_NOTICES
#define CVUI_IMPLEMENTATION
#include "cvui.h"

CCommunication::CCommunication()
{
    std::thread t1(&CCommunication::server_thread, this, &server);
    t1.detach();

    receiving = false;

    dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

    //pipeline parameters
    /*
    int capture_width = 1280; //480
    int capture_height =720; // 640
    int framerate = 15;
    int display_width = 640; // 480
    int display_height = 480; // 640
    */

    // reset frame average
    //std::string pipeline = gstreamer_pipeline(capture_width, capture_height, framerate, display_width, display_height);
    //std::cout << "Using pipeline: \n\t" << pipeline << "\n\n\n";
    cv::namedWindow("Camera");

    //_cap = cv::VideoCapture(pipeline, cv::CAP_GSTREAMER);
    _cap = cv::VideoCapture(0);
    if(!_cap.isOpened())
    {
        std::cout << "Failed to open camera" << std::endl;
    }

    std::thread t2(&CCommunication::img_server_thread, this, &img_server);
    t2.detach();

    // Create a window for displaying the camera feed and cvui controls
    cv::namedWindow("Camera");

     // Initialize cvui
    cvui::init("Camera");

    // targetID = 32;

    automatic = false;

    dist.push_back(1000.0);
}

CCommunication::~CCommunication()
{
    server.stop();
    _cap.release();
    img_server.stop();
    cv::destroyAllWindows();
}

void CCommunication::communicate()
{
    server.get_cmd(cmds);
    if(cmds.size() > 0)
    {
        receiving = true;
        if (cmds.at(0) != "n")
            key = cmds.at(0);
    } else {
        receiving = false;}

    cv::Mat frame;
    //_cap >> frame;
    _cap.read(frame);

    cv::rotate(frame, _display_im, cv::ROTATE_90_CLOCKWISE);

    if(_display_im.empty() == true)
        std::cout << "Capture read error" << std::endl;

    // Rotate the image to the correct orientation


    std::vector<std::vector<cv::Point2f>> corners, rejectedCandidates;
    cv::aruco::DetectorParameters detectorParams= cv::aruco::DetectorParameters();
    cv::aruco::ArucoDetector detector(dictionary, detectorParams);
    detector.detectMarkers(_display_im, corners, ids, rejectedCandidates);
    if (ids.size() > 0)
    {
            pixels_off.clear();
            dist.clear();
            for (size_t i = 0; i < ids.size(); ++i)
                {
                        // Find the minimum and maximum points
                        cv::Point minPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
                        cv::Point maxPoint(std::numeric_limits<int>::min(), std::numeric_limits<int>::min());
                        for (const auto& point : corners[i]) {
                            minPoint.x = std::min(minPoint.x, static_cast<int>(point.x));
                            minPoint.y = std::min(minPoint.y, static_cast<int>(point.y));
                            maxPoint.x = std::max(maxPoint.x, static_cast<int>(point.x));
                            maxPoint.y = std::max(maxPoint.y, static_cast<int>(point.y));
                        }


                        pixels_off.push_back(off_center(minPoint, maxPoint, _display_im));

                        // Draw lines connecting the corner points to form the outline of the marker
                        if (abs(pixels_off.at(i)) < 30)
                        {
                            for (int j = 0; j < 4; ++j) {
                                cv::line(_display_im, corners[i][j], corners[i][(j + 1) % 4], cv::Scalar(0, 0, 255), 2);
                            }
                           cv::line(_display_im, corners[0][0], corners[0][2], cv::Scalar(0, 0, 255), 2);
                           cv::line(_display_im, corners[0][1], corners[0][3], cv::Scalar(0, 0, 255), 2);
                        }
                        else {
                            for (int j = 0; j < 4; ++j) {
                                cv::line(_display_im, corners[i][j], corners[i][(j + 1) % 4], cv::Scalar(0, 255, 0), 2);
                            }
                        }

                        int area = ((corners[i][0].x*corners[i][1].y - corners[i][0].y*corners[i][1].x)
                                    +(corners[i][1].x*corners[i][2].y - corners[i][1].y*corners[i][2].x)
                                    +(corners[i][2].x*corners[i][3].y - corners[i][2].y*corners[i][3].x)
                                    +(corners[i][3].x*corners[i][0].y - corners[i][3].y*corners[i][0].x))/2;
                        dist.push_back(9.6278e2 * std::pow(area, -0.46935));

                        // Write the ID near the marker
                        cv::putText(_display_im, "ID: " + std::to_string(ids[i]), cv::Point(minPoint.x, minPoint.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
                        cv::putText(_display_im, "Area: " + std::to_string(area), cv::Point(minPoint.x, minPoint.y - 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
                        cv::putText(_display_im, "Distance (cm): " + std::to_string(dist.at(i)), cv::Point(minPoint.x, minPoint.y - 50), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
                }
    }

    //show the camera output (debug only, remove in final build)
    cv::imshow("Camera", _display_im);
    cv::waitKey(1);

    // Only send an image if in manual mode
    if(!automatic)
    {
        img_server.get_cmd(img_cmds);
        if(_cap.isOpened() == true)
        {
            if(_display_im.empty() == false)
                img_server.set_txim(_display_im);
        }
    }

}

void CCommunication::server_thread(CServer* server)
{
    // Start server
    server->start(4618);
}

void CCommunication::img_server_thread(CServer* server)
{
    // Start image server
    img_server.start(4699);
}

/*
std::string CCommunication::gstreamer_pipeline(int capture_width, int capture_height, int framerate, int display_width, int display_height)
{
   return
      " libcamerasrc ! video/x-raw, "
      " width=(int)" + std::to_string(capture_width) + ","
      " height=(int)" + std::to_string(capture_height) + ","
      " framerate=(fraction)" + std::to_string(framerate) + "/1 !"
      " videoconvert ! videoscale !"
      " video/x-raw,"
      " width=(int)" + std::to_string(display_width) + ","
      " height=(int)" + std::to_string(display_height) + " ! appsink";
};
*/

int CCommunication::off_center(cv::Point minPoint, cv::Point maxPoint, cv::Mat image)
{
    // Column that corresponds to the center of the image
    int center = image.cols/2;

    // Centre of the aruco marker
    int mark_center = (maxPoint.x + minPoint.x)/2;

    return mark_center - center;
}

