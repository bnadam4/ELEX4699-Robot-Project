#include "Communication.h"

CCommunication::CCommunication()
{
    std::thread t1(&CCommunication::server_thread, this, &server);
    t1.detach();

    receiving = false;

    dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

    //pipeline parameters
    int capture_width = 640; //1280
    int capture_height =480; // 720
    int framerate = 15;
    int display_width = 640; // 1280
    int display_height = 480; // 720

    // reset frame average
    std::string pipeline = gstreamer_pipeline(capture_width, capture_height, framerate, display_width, display_height);
    std::cout << "Using pipeline: \n\t" << pipeline << "\n\n\n";

    _cap = cv::VideoCapture(pipeline, cv::CAP_GSTREAMER);
    if(!_cap.isOpened())
    {
        std::cout << "Failed to open camera" << std::endl;
    }

    std::thread t2(&CCommunication::img_server_thread, this, &img_server);
    t2.detach();
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

    _cap >> _display_im;

    if(_display_im.empty() == true)
        std::cout << "Capture read error" << std::endl;

    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f>> corners, rejectedCandidates;
    cv::aruco::DetectorParameters detectorParams= cv::aruco::DetectorParameters();
    cv::aruco::ArucoDetector detector(dictionary, detectorParams);
    detector.detectMarkers(_display_im, corners, ids, rejectedCandidates);
    if(ids.size() > 0)
    {
        cv::Mat outputImage = _display_im.clone();
        cv::aruco::drawDetectedMarkers(outputImage, corners, ids);
        _display_im = outputImage;
        std::cout << ids.at(0) << std::endl;
        std::string id_string = "Ids detected: ";
        for(int i=0; i < ids.size(); i++)
        {
            id_string += " " + std::to_string(ids.at(i));
        }
        // server.send_string(id_string);
    }

    //show the camera output
    cv::imshow("Camera", _display_im);
    cv::waitKey(1);

    img_server.get_cmd(img_cmds);
    if(_cap.isOpened() == true)
    {
        if(_display_im.empty() == false)
            img_server.set_txim(_display_im);
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

