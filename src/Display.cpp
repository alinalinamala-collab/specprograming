#include "Display.hpp"

Display::Display() {
    name = "Lab 4 - AI Face & Gender";
    // Створюємо просте вікно без слайдерів
    cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
}

Display::~Display() {
    cv::destroyWindow(name);
}

void Display::showFrame(const cv::Mat& img) {
    if (!img.empty()) {
        cv::imshow(name, img);
    }
}
