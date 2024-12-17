cmake_minimum_required(VERSION 3.28)
project(Fluids)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS "-Wall -std=c++23 -DTYPES=\"FIXED(32,16)\"")

add_executable(Fluids
    main.cpp)
