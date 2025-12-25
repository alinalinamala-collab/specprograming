#pragma once
#include <opencv2/opencv.hpp>

class Camera {
    cv::VideoCapture cap;
public:
    Camera();
    cv::Mat getNextFrame();
};
