#!/bin/bash 

g++ \
    --std=c++17 \
    -fPIC --shared \
    -O0 \
    main.cpp \
    -I/usr/include \
    -I/usr/include/python${PYTHON_VERSION} \
    -lboost_python${PYTHON_VERSION/./} \
    -lpython${PYTHON_VERSION} \
    -o foo.so