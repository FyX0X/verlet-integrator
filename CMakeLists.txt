cmake_minimum_required(VERSION 3.28)
project(VerletIntegrator LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(verlet src/main.cpp "src/VerletIntegrator.cpp" "include/Point.h"  "include/Link.h" "src/Link.cpp" "src/Renderer.cpp")
target_include_directories(verlet PRIVATE "include")
target_compile_features(verlet PRIVATE cxx_std_17)
target_link_libraries(verlet PRIVATE SFML::Graphics)
