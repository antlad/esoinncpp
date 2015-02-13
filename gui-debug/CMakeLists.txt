project(gui-debug)
cmake_minimum_required(VERSION 2.8)

set(GUITEST_LIST
ui/mainform.cpp
ui/mainform.h
data/nodesmodel.h
data/nodesmodel.cpp
data/ocidataset.h
data/ocidataset.cpp
#data/filter.cpp
#data/kaggleparser.cpp
#data/filter.h
#data/kaggleparser.h
main.cpp
)

find_package(esoinncpp REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

qt5_wrap_ui(UIS_HDRS
	ui/mainform.ui
)

include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Core_INCLUDES})

if (NOT MSVC)
add_definitions(-std=c++11)
endif()


add_executable(gui-debug ${GUITEST_LIST} ${UIS_HDRS})
target_link_libraries(gui-debug ${esoinncpp_LIBRARIES})
qt5_use_modules(gui-debug  Widgets Core)

include_directories( ${esoinncpp_INCLUDE_DIRS})

#include_directories( ${esoinncpp_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
#target_link_libraries(kagglediggits ${esoinncpp_LIBRARIES} ${Boost_LIBRARIES} ${PNG_LIBRARY})
#target_link_libraries(ocitest ${esoinncpp_LIBRARIES} ${Boost_LIBRARIES} ${PNG_LIBRARY})

