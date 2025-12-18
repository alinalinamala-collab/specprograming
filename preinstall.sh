#!/bin/bash
echo "Встановлення бібліотек..."
sudo apt update
sudo apt install libopencv-dev cmake g++ build-essential wget -y

echo "1. Завантаження детектора облич (Face Detector)..."
wget -nc https://raw.githubusercontent.com/opencv/opencv/master/samples/dnn/face_detector/deploy.prototxt
wget -nc https://raw.githubusercontent.com/opencv/opencv_3rdparty/dnn_samples_face_detector_20170830/res10_300x300_ssd_iter_140000.caffemodel

echo "2. Завантаження детектора статі (Gender Detector)..."
# Видаляємо старі/биті файли, якщо вони є
rm -f gender_deploy.prototxt gender_net.caffemodel

# Завантажуємо з надійного дзеркала (GitHub репозиторій smahesh29)
wget -O gender_deploy.prototxt https://raw.githubusercontent.com/smahesh29/Gender-and-Age-Detection/master/gender_deploy.prototxt
wget -O gender_net.caffemodel https://github.com/smahesh29/Gender-and-Age-Detection/raw/master/gender_net.caffemodel

echo "Готово! Усі файли завантажено."
