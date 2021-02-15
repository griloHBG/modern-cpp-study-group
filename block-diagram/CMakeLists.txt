cmake_minimum_required(VERSION 3.17)
project(BlockDiagramBuilder)

set(CMAKE_CXX_STANDARD 20)

add_executable(BlockDiagramBuilder main.cpp BlockBase.h BlockSum.h Signal.h BlockConstant.h utils_debug.h BlockIndexCtorCounter.h)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic) # -Werror)

target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:DEBUG>")
