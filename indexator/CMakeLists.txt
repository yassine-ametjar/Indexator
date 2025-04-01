cmake_minimum_required(VERSION 3.10)
project(indexator)

set(CMAKE_CXX_STANDARD 17)

add_executable(indexator
        main.cpp
        Element.h
        Node.h
        Index.h
        IndexManager.h
)