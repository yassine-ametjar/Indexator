cmake_minimum_required(VERSION 3.30)
project(indexator)

set(CMAKE_CXX_STANDARD 20)

add_executable(indexator main.cpp
        Node.h
        Element.h
        Index.h)
