cmake_minimum_required(VERSION 3.25)

project(
    Calibra
    VERSION 0.1.0
    DESCRIPTION "Calibration and diagnostics library"
    LANGUAGES CXX
)

set(CMAKE_CXX_EXTENSIONS OFF)

add_library(
    calibra_core
    cpp/src/Model.cpp
)

target_compile_features(
    calibra_core
    PUBLIC
        cxx_std_20
)

target_include_directories(
    calibra_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/cpp/include
)