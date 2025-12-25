#pragma once
#include <opencv2/opencv.hpp>

class Filters {
public:
    // Режими роботи: 0-норм, 1-сірий, 2-блюр, 3-краї, 4-інверсія
    static void applyEffect(cv::Mat &img, int mode, int value);
};
