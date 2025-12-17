#!/bin/bash
echo "Встановлення бібліотек..."
sudo apt update
sudo apt install libopencv-dev cmake g++ build-essential wget -y

echo "1. Завантаження детектора облич..."
wget -nc https://raw.githubusercontent.com/opencv/opencv/master/samples/dnn/face_detector/deploy.prototxt
wget -nc https://raw.githubusercontent.com/opencv/opencv_3rdparty/dnn_samples_face_detector_20170830/res10_300x300_ssd_iter_140000.caffemodel

echo "2. Завантаження детектора статі..."
wget -nc https://raw.githubusercontent.com/opencv/opencv/master/samples/dnn/gender_detector/gender_deploy.prototxt
wget -nc https://raw.githubusercontent.com/opencv/opencv_3rdparty/dnn_samples_face_detector_20170830/gender_net.caffemodel

echo "Готово!"
