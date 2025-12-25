#include "FaceDetector.hpp"
#include <iostream>

FaceDetector::FaceDetector() : running(true), hasNewFrame(false) {
    // Завантаження мереж
    faceNet = cv::dnn::readNetFromCaffe(
        "deploy.prototxt", 
        "res10_300x300_ssd_iter_140000.caffemodel"
    );
    
    genderNet = cv::dnn::readNetFromCaffe(
        "gender_deploy.prototxt", 
        "gender_net.caffemodel"
    );

    // Запуск фонового потоку
    worker = std::thread(&FaceDetector::loop, this);
}

FaceDetector::~FaceDetector() {
    running = false;
    condition.notify_one(); // Розбудити потік, щоб він міг завершитись
    if (worker.joinable()) {
        worker.join();
    }
}

void FaceDetector::updateFrame(const cv::Mat& frame) {
    // Швидко копіюємо кадр під захистом
    {
        std::lock_guard<std::mutex> lock(mtx);
        frame.copyTo(currentFrame);
        hasNewFrame = true;
    }
    condition.notify_one(); // Кажемо потоку: "Є робота!"
}

std::vector<FaceInfo> FaceDetector::getResults() {
    // Швидко віддаємо останні відомі результати
    std::lock_guard<std::mutex> lock(mtx);
    return results;
}

void FaceDetector::loop() {
    cv::Mat frameToProcess;
    
    while (running) {
        // 1. ЧЕКАННЯ І КОПІЮВАННЯ (Дуже швидко)
        {
            std::unique_lock<std::mutex> lock(mtx);
            condition.wait(lock, [this]{ return hasNewFrame || !running; });
            
            if (!running) break;
            
            // Копіюємо кадр у локальну змінну і відразу звільняємо м'ютекс!
            frameToProcess = currentFrame.clone();
            hasNewFrame = false;
        } 
        // ТУТ М'ЮТЕКС ВЖЕ РОЗБЛОКОВАНО!
        // Main Thread може спокійно малювати інтерфейс.

        // 2. ВАЖКІ ОБЧИСЛЕННЯ (Довго)
        std::vector<FaceInfo> currentFaces = detect(frameToProcess);

        // 3. ЗБЕРЕЖЕННЯ РЕЗУЛЬТАТУ (Дуже швидко)
        {
            std::lock_guard<std::mutex> lock(mtx);
            results = currentFaces;
        }
    }
}

std::vector<FaceInfo> FaceDetector::detect(cv::Mat& img) {
    std::vector<FaceInfo> faces;
    if (img.empty()) return faces;

    // --- Face Detection ---
    cv::Mat blob = cv::dnn::blobFromImage(img, 1.0, cv::Size(300, 300), cv::Scalar(104, 177, 123));
    faceNet.setInput(blob);
    cv::Mat detection = faceNet.forward();

    cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

    for (int i = 0; i < detectionMat.rows; i++) {
        float confidence = detectionMat.at<float>(i, 2);

        if (confidence > 0.5) {
            int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * img.cols);
            int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * img.rows);
            int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * img.cols);
            int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * img.rows);

            // Перевірка меж
            x1 = std::max(0, x1); y1 = std::max(0, y1);
            x2 = std::min(img.cols - 1, x2); y2 = std::min(img.rows - 1, y2);
            
            cv::Rect box(x1, y1, x2 - x1, y2 - y1);
            
            // --- Gender Detection ---
            // Вирізаємо обличчя
            cv::Mat faceROI = img(box);
            if (faceROI.empty()) continue;

            cv::Mat genderBlob = cv::dnn::blobFromImage(faceROI, 1.0, cv::Size(227, 227), 
                                                      cv::Scalar(78.4263377603, 87.7689143744, 114.895847746), 
                                                      false);
            genderNet.setInput(genderBlob);
            cv::Mat genderPreds = genderNet.forward();
            
            float maleConf = genderPreds.at<float>(0, 0);
            float femaleConf = genderPreds.at<float>(0, 1);
            
            std::string label = (maleConf > femaleConf) ? "Male" : "Female";
            faces.push_back({box, label});
        }
    }
    return faces;
}
