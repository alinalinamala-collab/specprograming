#include "Camera.hpp"
#include "Display.hpp"
#include "FaceDetector.hpp"
#include <iostream>

int main() {
    Camera cam;
    Display disp; // Використовуємо ваш старий клас Display
    FaceDetector detector;

    bool useAI = false;

    std::cout << "=== Lab 4, Gender Detection ===" << std::endl;
    std::cout << "Натисніть 'F' для увімкнення AI" << std::endl;
    std::cout << "Натисніть 'ESC' для виходу" << std::endl;

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

