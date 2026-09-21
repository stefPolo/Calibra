cmake_minimum_required(VERSION 3.25)

project(
    Calibra
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_EXTENSIONS OFF)

add_library(
    calibra
    cpp/src/LinearModel.cpp
    cpp/src/ParameterSpace.cpp
    cpp/src/MeanSquaredError.cpp
)

target_compile_features(
    calibra
    PUBLIC
        cxx_std_20
)

target_include_directories(
    calibra
    PUBLIC
        ${PROJECT_SOURCE_DIR}/cpp/include
)

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------

add_executable(
    test_calibration_problem
    cpp/tests/test_calibration_problem.cpp
)

target_link_libraries(
    test_calibration_problem
    PRIVATE
        calibra
)

# ---------------------------------------------------------------------------
# CTest
# ---------------------------------------------------------------------------

add_test(
    NAME calibration_problem
    COMMAND test_calibration_problem
)