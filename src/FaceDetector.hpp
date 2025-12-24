#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <string>
#include <condition_variable>

// Структура: координати + підпис (хто це)
struct FaceInfo {
    cv::Rect rect;
    std::string label;
};

class FaceDetector {
public:
    FaceDetector();
    ~FaceDetector();

    void updateFrame(const cv::Mat& frame);
    std::vector<FaceInfo> getResults();

private:
    cv::dnn::Net faceNet;   // Нейромережа для облич
    cv::dnn::Net genderNet; // Нейромережа для статі

    std::thread worker;
    std::atomic<bool> running;
    std::mutex mtx;
    std::condition_variable condition;

    cv::Mat currentFrame;
    std::vector<FaceInfo> results;
    bool hasNewFrame;

    void loop();
    std::vector<FaceInfo> detect(cv::Mat& img);
};
