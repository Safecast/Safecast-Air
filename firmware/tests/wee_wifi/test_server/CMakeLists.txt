cmake_minimum_required (VERSION 2.6)

project(weewifi_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable( weewifi_test weewifi_test.cpp)

target_link_libraries(weewifi_test boost_system pthread)



