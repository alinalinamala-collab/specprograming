cmake_minimum_required(VERSION 3.10)
project(Lab4App)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

# === ВИПРАВЛЕННЯ ТУТ ===
# Ми явно додаємо шлях до заголовних файлів OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})
# =======================

include_directories(src)
file(GLOB SOURCES "src/*.cpp")

add_executable(Lab4App ${SOURCES})
target_link_libraries(Lab4App ${OpenCV_LIBS} Threads::Threads)

project(Lab3Simple)
set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
include_directories(src)
file(GLOB SOURCES "src/*.cpp")
add_executable(Lab3Simple ${SOURCES})
target_link_libraries(Lab3Simple ${OpenCV_LIBS})
