#include "FaceDetector.hpp"
#include <iostream>

FaceDetector::FaceDetector() : isRunning(true), newFrameAvailable(false) {
    // Завантажуємо обидві нейромережі
    faceNet = cv::dnn::readNetFromCaffe("deploy.prototxt", "res10_300x300_ssd_iter_140000.caffemodel");
    genderNet = cv::dnn::readNetFromCaffe("gender_deploy.prototxt", "gender_net.caffemodel");

    if (faceNet.empty() || genderNet.empty()) {
        std::cerr << "Увага: Не знайдено моделі! Запустіть ./preinstall.sh" << std::endl;
    }

    workerThread = std::thread(&FaceDetector::workerLoop, this);
}

FaceDetector::~FaceDetector() {
    isRunning = false;
    if (workerThread.joinable()) workerThread.join();
}

void FaceDetector::updateFrame(const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(dataMutex);
    currentFrame = frame.clone();
    newFrameAvailable = true;
}

std::vector<FaceInfo> FaceDetector::getResults() {
    std::lock_guard<std::mutex> lock(dataMutex);
    return detectedResults;
}

void FaceDetector::workerLoop() {
    // Середні значення кольорів для навчання цієї моделі
    const cv::Scalar meanVal(78.4263377603, 87.7689143744, 114.895847746);

    while (isRunning) {
        cv::Mat frameToProcess;
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            if (!newFrameAvailable) {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                continue;
            }
            frameToProcess = currentFrame.clone();
            newFrameAvailable = false;
        }

        if (frameToProcess.empty()) continue;

        // 1. Шукаємо обличчя
        cv::Mat blob = cv::dnn::blobFromImage(frameToProcess, 1.0, cv::Size(300, 300), cv::Scalar(104, 177, 123));
        faceNet.setInput(blob);
        cv::Mat detections = faceNet.forward();

        std::vector<FaceInfo> tempResults;
        cv::Mat detectionMat(detections.size[2], detections.size[3], CV_32F, detections.ptr<float>());

        for (int i = 0; i < detectionMat.rows; i++) {
            float confidence = detectionMat.at<float>(i, 2);
            if (confidence > 0.6) { // Поріг впевненості 60%
                int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frameToProcess.cols);
                int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frameToProcess.rows);
                int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frameToProcess.cols);
                int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frameToProcess.rows);

                cv::Rect faceRect(cv::Point(x1, y1), cv::Point(x2, y2));
                // Захист від виходу за межі кадру
                faceRect = faceRect & cv::Rect(0, 0, frameToProcess.cols, frameToProcess.rows);

                // Якщо обличчя надто мале - ігноруємо
                if (faceRect.width < 10 || faceRect.height < 10) continue;

                // 2. Визначаємо стать
                cv::Mat faceROI = frameToProcess(faceRect); // Вирізаємо обличчя
                cv::Mat genderBlob = cv::dnn::blobFromImage(faceROI, 1.0, cv::Size(227, 227), meanVal, false);

                genderNet.setInput(genderBlob);
                cv::Mat genderPreds = genderNet.forward();

                // 0 = Male, 1 = Female
                float maleProb = genderPreds.at<float>(0, 0);
                float femaleProb = genderPreds.at<float>(0, 1);

                std::string label = (maleProb > femaleProb) ? "Man" : "Woman";

                // Додаємо структуру в список
                tempResults.push_back({faceRect, label});
            }
        }

        // Зберігаємо результати
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            detectedResults = tempResults;
        }
    }
}
