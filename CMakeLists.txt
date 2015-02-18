cmake_minimum_required(VERSION 2.6)
project(Mathador)




file(GLOB MathadorSources ${PROJECT_SOURCE_DIR}/*.cpp)

add_executable(mathador ${MathadorSources})
