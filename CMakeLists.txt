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
    test_linear_model
    cpp/tests/test_linear_model.cpp
)

target_link_libraries(
    test_linear_model
    PRIVATE
        calibra
)

add_executable(
    test_parameter_space
    cpp/tests/test_parameter_space.cpp
)

target_link_libraries(
    test_parameter_space
    PRIVATE
        calibra
)

add_executable(
    test_mse
    cpp/tests/test_mse.cpp
)

target_link_libraries(
    test_mse
    PRIVATE
        calibra
)

# ---------------------------------------------------------------------------
# CTest
# ---------------------------------------------------------------------------

enable_testing()

add_test(
    NAME linear_model
    COMMAND test_linear_model
)

add_test(
    NAME parameter_space
    COMMAND test_parameter_space
)

add_test(
    NAME mse
    COMMAND test_mse
)