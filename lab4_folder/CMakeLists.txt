cmake_minimum_required(VERSION 3.10)
project(Lab4App)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

# Додаємо шлях до папки src, щоб CMake бачив ваші .hpp файли
include_directories(src ${OpenCV_INCLUDE_DIRS})

# Список усіх файлів (вже з правильною латинською "c")
add_executable(Lab4App 
    src/main.cpp 
    src/Camera.cpp 
    src/FaceDetector.cpp 
    src/Filters.cpp
    src/Display.cpp
)

target_link_libraries(Lab4App ${OpenCV_LIBS} Threads::Threads)
