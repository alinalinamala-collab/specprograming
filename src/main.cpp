#include "Camera.hpp"
#include "Display.hpp"
#include "FaceDetector.hpp"
#include "Filters.hpp"
#include "Display.hpp"
#include <iostream>

int main() {
    Camera cam;
    Display disp; // Використовуємо ваш старий клас Display
    FaceDetector detector;

    bool useAI = false;

    std::cout << "=== Lab 4, Gender Detection ===" << std::endl;
    std::cout << "Натисніть 'F' для увімкнення AI" << std::endl;
    std::cout << "Натисніть 'ESC' для виходу" << std::endl;

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
        if (useAI) {
            // Відправляємо кадр на обробку (це не блокує програму)
            detector.updateFrame(frame);

            // Миттєво отримуємо результат
            std::vector<FaceInfo> results  = detector.getResults();

            // Малюємо обличчя
            for (const auto& info : results) {
                cv::rectangle(frame, info.rect, cv::Scalar(255, 100, 0), 2);

               std::string text = info.label;
                int baseLine;
                cv::Size labelSize = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.7, 2, &baseLine);
                int y = std::max(info.rect.y, labelSize.height);

                cv::rectangle(frame, cv::Point(info.rect.x, y - labelSize.height),
                              cv::Point(info.rect.x + labelSize.width, y + baseLine),
                              cv::Scalar(255, 100, 0), cv::FILLED);

                // Текст
                cv::putText(frame, text, cv::Point(info.rect.x, y), 
                           cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);
            }
            cv::putText(frame, "AI: ON (Async)", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
        } else {
            cv::putText(frame, "AI: OFF", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 2);
        }

        // Використовуємо старий метод для відображення
        disp.showFrame(frame);

        int key = cv::waitKey(30);
        if (key == 27) break;
        if (key == 'f' || key == 'F') useAI = !useAI;
    }
    return 0;
}


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
