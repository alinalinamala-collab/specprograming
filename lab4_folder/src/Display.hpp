#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

struct Shape {
    int type; // 1 = Лінія, 2 = Прямокутник
    cv::Point start;
    cv::Point end;
};

class Display {
public:
    std::string name;
    
    static int sliderValue;
    static int drawMode; 
    static bool isMousePressed;
    static cv::Point startPoint;
    static cv::Point currentPoint;
    static std::vector<Shape> shapes;

    Display(const std::string& windowName = "Lab 4 App");
    ~Display();

    void showFrame(cv::Mat img); // Приберіть const тут, якщо в .cpp його немає
    static void drawOverlay(cv::Mat &img); 
    static void clearShapes();
    static void onMouse(int event, int x, int y, int flags, void* userdata);
};
