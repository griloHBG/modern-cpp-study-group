cmake_minimum_required(VERSION 3.16)
project(ft_sensor_read)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic) # -Werror)
