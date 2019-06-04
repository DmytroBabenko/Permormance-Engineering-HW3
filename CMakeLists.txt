cmake_minimum_required(VERSION 3.14)
project(HW3)

set(CMAKE_CXX_STANDARD 17)

add_executable(HW3 main.cpp BMPImage.h Worker.h Worker.cpp ThreadPool.h ThreadPool.cpp Reducer.hpp Reducer.h IntegralImage.cpp IntegralImage.h)