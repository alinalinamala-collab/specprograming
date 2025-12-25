#include "Filters.hpp"

void Filters::applyEffect(cv::Mat &img, int mode, int value) {
    // value - це значення слайдера (0-100)
    // Переконаємось, що значення не 0 для математики
    int param = (value < 1) ? 1 : value;

    if (mode == 1) { // Сірий + Поріг (Threshold)
        cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
        cv::threshold(img, img, param * 2.5, 255, cv::THRESH_BINARY);
        cv::cvtColor(img, img, cv::COLOR_GRAY2BGR); // Повертаємо канали, щоб не було помилки відображення
    }
    else if (mode == 2) { // Розмиття (Blur)
        // param має бути непарним
        int k = param % 2 == 0 ? param + 1 : param;
        cv::GaussianBlur(img, img, cv::Size(k, k), 0);
    }
    else if (mode == 3) { // Детектор країв (Canny)
        cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
        cv::Canny(img, img, param, param * 2);
        cv::cvtColor(img, img, cv::COLOR_GRAY2BGR);
    }
    else if (mode == 4) { // Інверсія
        cv::bitwise_not(img, img);
    }
    
    // Відобразимо режим на екрані
    cv::putText(img, "Mode: " + std::to_string(mode), cv::Point(10, 30), 
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
}
