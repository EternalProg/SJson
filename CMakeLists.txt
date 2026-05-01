cmake_minimum_required(VERSION 3.30)
project(SJson)

add_executable(SJson main.cpp src/JsonConverter.cpp src/JsonValue.cpp)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_include_directories(SJson PUBLIC headers)
