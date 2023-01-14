cmake_minimum_required(VERSION 3.22)
project(GeometricTransformation)

set(CMAKE_CXX_STANDARD 14)

add_executable(GeometricTransformation main.cpp bmp.h bmp.cpp GeoTransform.cpp GeoTransform.h)
