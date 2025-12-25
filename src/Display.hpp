#pragma once
#include <opencv2/opencv.hpp>
lab4
#include <string>

class Display {
public:
    std::string name;

    Display();
    ~Display();

    void showFrame(const cv::Mat& img);
#include <vector>
#include <string>

// Структура для зберігання фігури
struct Shape {
    int type; // 1 = Лінія, 2 = Прямокутник
    cv::Point start;
    cv::Point end;
};

class Display {
public:
    std::string name;
    static int sliderValue;
    
    // Змінні для малювання
    static int drawMode; // 0=Ні, 1=Лінія, 2=Прямокутник
    static bool isMousePressed;
    static cv::Point startPoint;
    static cv::Point currentPoint;
    static std::vector<Shape> shapes; // Список всіх фігур

    Display();
    void showFrame(cv::Mat img);
    
    // Метод, який накладає фігури на кадр
    static void drawOverlay(cv::Mat &img); 
    
    // Очищення малюнків
    static void clearShapes();

    static void onMouse(int event, int x, int y, int flags, void* userdata);
main
};
