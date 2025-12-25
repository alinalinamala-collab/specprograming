#!/bin/bash
if [ -f "./build/Lab3Simple" ]; then
    ./build/Lab3Simple
else
    echo "Спочатку запустіть ./build.sh"
fi
