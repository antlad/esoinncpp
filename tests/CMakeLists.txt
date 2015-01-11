project(kagglediggits)
cmake_minimum_required(VERSION 2.8)

SET(SRC_LIST
main.cpp
kaggleparser.h
kaggleparser.cpp
)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS REQUIRED filesystem system)
find_package(esoinncpp REQUIRED)

if (NOT MSVC)
add_definitions(-std=c++11)
endif()

add_executable(kagglediggits ${SRC_LIST})

include_directories( ${esoinncpp_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
target_link_libraries(kagglediggits ${esoinncpp_LIBRARIES} ${Boost_LIBRARIES})

