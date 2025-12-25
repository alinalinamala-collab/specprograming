#include "Display.hpp"

// ІНІЦІАЛІЗАЦІЯ (без цього буде помилка "undefined reference")
int Display::sliderValue = 0;
int Display::drawMode = 0;
bool Display::isMousePressed = false;
cv::Point Display::startPoint = cv::Point(0,0);
cv::Point Display::currentPoint = cv::Point(0,0);
std::vector<Shape> Display::shapes = {};

Display::Display(const std::string& windowName) {
    name = windowName;
    cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
    // Важливо: встановлюємо обробник миші
    cv::setMouseCallback(name, Display::onMouse, NULL);
}

Display::~Display() {
    cv::destroyWindow(name);
}

void Display::showFrame(cv::Mat img) {
    if (!img.empty()) {
        // Накладаємо малюнки перед показом
        drawOverlay(img);
        cv::imshow(name, img);
    }
}

// Додайте порожні або робочі реалізації для інших методів, щоб не було помилок
void Display::drawOverlay(cv::Mat &img) {}
void Display::clearShapes() { shapes.clear(); }
void Display::onMouse(int event, int x, int y, int flags, void* userdata) {}
