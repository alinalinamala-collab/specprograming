#include "Camera.hpp"
#include "Display.hpp"
#include "FaceDetector.hpp"
#include <iostream>

int main() {
    Camera cam;
    Display disp;
    FaceDetector detector;
    bool useAI = false;

    std::cout << "=== Gender Detection ===" << std::endl;
    std::cout << "Натисніть 'F' щоб увімкнути AI" << std::endl;

    while (true) {
        cv::Mat frame = cam.getNextFrame();
        if (frame.empty()) break;

        if (useAI) {
            detector.updateFrame(frame);

            // Отримуємо список облич з підписами
            std::vector<FaceInfo> results = detector.getResults();

            for (const auto& info : results) {
                // Рамка
                cv::rectangle(frame, info.rect, cv::Scalar(255, 100, 0), 2);

                // Підкладка під текст (для краси)
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
            cv::putText(frame, "AI: Gender Mode", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
        } else {
            cv::putText(frame, "AI: OFF", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 2);
        }

        disp.showFrame(frame);
        int key = cv::waitKey(30);
        if (key == 27) break;
        if (key == 'f' || key == 'F') useAI = !useAI;
    }
    return 0;
}
