#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <string>

// Структура: координати + підпис (хто це)
struct FaceInfo {
    cv::Rect rect;
    std::string label;
};

class FaceDetector {
private:
    cv::dnn::Net faceNet;   // Нейромережа для облич
    cv::dnn::Net genderNet; // Нейромережа для статі

    std::thread workerThread;
    std::atomic<bool> isRunning;
    std::mutex dataMutex;

    cv::Mat currentFrame;
    std::vector<FaceInfo> detectedResults;
    bool newFrameAvailable;

    void workerLoop();

public:
    FaceDetector();
    ~FaceDetector();

    void updateFrame(const cv::Mat& frame);
    std::vector<FaceInfo> getResults();
};
