cmake_minimum_required(VERSION 3.22)
project(DIP)

set(CMAKE_CXX_STANDARD 14)

add_executable(DIP main.cpp bmp.cpp bmp.h GeoTransform.cpp GeoTransform.h Filtering.cpp Filtering.h Laplace.cpp Laplace.h BilateralFilter.cpp BilateralFilter.h)
