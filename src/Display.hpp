#pragma once
#include <opencv2/opencv.hpp>
#include <string>

class Display {
public:
    std::string name;

    Display();
    ~Display();

    void showFrame(const cv::Mat& img);
};
