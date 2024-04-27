#include "Guidance.h"

CGuidance::CGuidance()
{
    _cap.open(0, cv::CAP_V4L);

    if(!_cap.isOpened())
    {
        std::cout << "Failed to open camera" << std::endl;
    }

    dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

    // cvui::init(CANVAS_NAME);
}

CGuidance::~CGuidance()
{
    _cap.release();
    cv::destroyAllWindows();
}

void CGuidance::update()
{
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
    }

    //show the camera output
    cv::imshow("Camera", _display_im);

    cv::waitKey(1);
}
