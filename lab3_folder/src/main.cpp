#include "Camera.hpp"
#include "Filters.hpp"
#include "Display.hpp"
#include <iostream>

int main() {
    Camera cam;
    Display disp;
    int currentMode = 0;

    std::cout << "=== КЕРУВАННЯ ===" << std::endl;
    std::cout << "[1-4] - Ефекти (Ч/Б, Блюр, Краї, Інверсія)" << std::endl;
    std::cout << "[L]   - Режим ЛІНІЙ (тягніть мишкою)" << std::endl;
    std::cout << "[R]   - Режим ПРЯМОКУТНИКІВ (тягніть мишкою)" << std::endl;
    std::cout << "[N]   - Вимкнути малювання (Normal)" << std::endl;
    std::cout << "[C]   - Очистити малюнки" << std::endl;
    std::cout << "[ESC] - Вихід" << std::endl;

    while (true) {
        cv::Mat frame = cam.getNextFrame();
        if (frame.empty()) break;

        // 1. Застосувати фільтр
        Filters::applyEffect(frame, currentMode, Display::sliderValue);

        // 2. Накласти малюнки (лінії/прямокутники)
        Display::drawOverlay(frame);

        // 3. Відобразити статус малювання
        std::string modeStr = "Draw: OFF";
        if (Display::drawMode == 1) modeStr = "Draw: LINE";
        if (Display::drawMode == 2) modeStr = "Draw: RECT";
        cv::putText(frame, modeStr, cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 255), 2);

        disp.showFrame(frame);

        int key = cv::waitKey(10);
        if (key == 27) break; // ESC

        // Фільтри
        if (key >= '0' && key <= '4') currentMode = key - '0';

        // Малювання
        if (key == 'l' || key == 'L') Display::drawMode = 1; // Line
        if (key == 'r' || key == 'R') Display::drawMode = 2; // Rect
        if (key == 'n' || key == 'N') Display::drawMode = 0; // None
        if (key == 'c' || key == 'C') Display::clearShapes(); // Clear
    }
    return 0;
}
