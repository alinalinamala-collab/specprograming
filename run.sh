#!/bin/bash
APP_PATH="./build/Lab4App"

if [ -f "$APP_PATH" ]; then
    echo "Запуск Lab4App..."
    $APP_PATH
else
    echo "Помилка: Файл $APP_PATH не знайдено!"
    echo "Ось що є в папці build:"
    ls ./build/
    echo "Спробуйте запустити ./build.sh ще раз і перевірте помилки."
fi
