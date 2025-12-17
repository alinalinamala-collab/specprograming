#include "Camera.hpp"
#include <iostream>

Camera::Camera() {
    // 0 - це індекс веб-камери
    cap.open(0);
    if (!cap.isOpened()) {
        std::cerr << "Помилка камери!" << std::endl;
        exit(1);
    }
}

cv::Mat Camera::getNextFrame() {
    cv::Mat frame;
    cap >> frame;
    return frame;
}
