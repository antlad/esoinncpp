project(gui-debug)
cmake_minimum_required(VERSION 2.8)

set(GUITEST_LIST
ui/mainform.cpp
ui/mainform.h
main.cpp
)

#add_definitions(-DBUILD_WITH_PNG_EXPORT_SUPPORT)
#set(Boost_USE_STATIC_LIBS ON)
#find_package(Boost COMPONENTS REQUIRED filesystem system)
#find_package(esoinncpp REQUIRED)
#find_package(PNG REQUIRED)


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

qt5_wrap_ui(UIS_HDRS
	ui/mainform.ui
)


#find_package(Qt5 COMPONENTS Widgets Core)
#find_package(Qt5Widgets REQUIRED)
#ind_package(Qt5Core REQUIRED)
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Core_INCLUDES})

if (NOT MSVC)
add_definitions(-std=c++11)
endif()


add_executable(gui-debug ${GUITEST_LIST} ${UIS_HDRS})
#target_link_libraries(gui-debug Qt5::Widgets Qt5::Core)
qt5_use_modules(gui-debug  Widgets Core)


#include_directories( ${esoinncpp_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
#target_link_libraries(kagglediggits ${esoinncpp_LIBRARIES} ${Boost_LIBRARIES} ${PNG_LIBRARY})
#target_link_libraries(ocitest ${esoinncpp_LIBRARIES} ${Boost_LIBRARIES} ${PNG_LIBRARY})

