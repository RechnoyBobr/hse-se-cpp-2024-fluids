cmake_minimum_required(VERSION 3.28)
project(Fluids)

set(CMAKE_CXX_STANDARD 23)


set(SIZES "S(34,84)" CACHE STRING "Sizes to precompile")
set(TYPES "FIXED(32,16),FAST_FIXED(16,8),FAST_FIXED(32,16)" CACHE STRING "Types to precompile")
add_compile_definitions(-DTYPES=${TYPES})
add_compile_definitions(-DSIZES=${SIZES})
set(CMAKE_CXX_FLAGS "-Wall -std=c++23")
add_executable(Fluids
        main.cpp)
