#include "Display.hpp"
#include <iostream>

int Display::sliderValue = 50;
int Display::drawMode = 0; // За замовчуванням малювання вимкнено
bool Display::isMousePressed = false;
cv::Point Display::startPoint;
cv::Point Display::currentPoint;
std::vector<Shape> Display::shapes;

Display::Display() {
    name = "Lab 3 Drawing";
    cv::namedWindow(name);
    cv::createTrackbar("Param", name, &sliderValue, 100);
    cv::setMouseCallback(name, onMouse, NULL);
}

void Display::showFrame(cv::Mat img) {
    if (!img.empty()) cv::imshow(name, img);
}

void Display::clearShapes() {
    shapes.clear();
}

void Display::drawOverlay(cv::Mat &img) {
    // 1. Малюємо вже збережені фігури
    for (const auto& s : shapes) {
        if (s.type == 1) { // Лінія
            cv::line(img, s.start, s.end, cv::Scalar(0, 0, 255), 2);
        } else if (s.type == 2) { // Прямокутник
            cv::rectangle(img, s.start, s.end, cv::Scalar(255, 0, 0), 2);
        }
    }

    // 2. Малюємо процес (поки тягнемо мишку)
    if (isMousePressed && drawMode != 0) {
        if (drawMode == 1)
            cv::line(img, startPoint, currentPoint, cv::Scalar(0, 255, 255), 1);
        else if (drawMode == 2)
            cv::rectangle(img, startPoint, currentPoint, cv::Scalar(255, 255, 0), 1);
    }
}

void Display::onMouse(int event, int x, int y, int flags, void* userdata) {
    if (drawMode == 0) return; // Якщо режим малювання вимкнено - ігноруємо

    if (event == cv::EVENT_LBUTTONDOWN) {
        isMousePressed = true;
        startPoint = cv::Point(x, y);
        currentPoint = cv::Point(x, y);
    } 
    else if (event == cv::EVENT_MOUSEMOVE) {
        if (isMousePressed) {
            currentPoint = cv::Point(x, y);
        }
    } 
    else if (event == cv::EVENT_LBUTTONUP) {
        isMousePressed = false;
        currentPoint = cv::Point(x, y);
        // Зберігаємо фігуру
        shapes.push_back({drawMode, startPoint, currentPoint});
    }
}
