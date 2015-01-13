cmake_minimum_required(VERSION 2.8)
project (esoinncpp)

set(ESOINN_SRC
src/esoinn.cpp
src/esoinn_node.cpp
src/utils.h
src/esoinn_node.h
)
set(INCLUDE_LIST
include/esoinn.h
include/esoinn_export.h
)

set(CMakeConfig_LIST esoinncppConfig.cmake)

add_library(${PROJECT_NAME} ${LIB_TYPE} ${ESOINN_SRC} ${INCLUDE_LIST} ${CMakeConfig_LIST})

option(PNG_EXPORT_SUPPORT "Build with png export support" OFF)

if(PNG_EXPORT_SUPPORT)
    find_package(PNG REQUIRED)
    find_package(Boost COMPONENTS REQUIRED filesystem system)
    include_directories(${Boost_INCLUDE_DIRS})
    add_definitions(-DBUILD_WITH_PNG_EXPORT_SUPPORT)
    target_link_libraries(${PROJECT_NAME} ${PNG_LIBRARY} ${Boost_LIBRARIES})
endif()

add_definitions(-DESOINN_EXPORT_DLL)

if (NOT MSVC)
    add_definitions(-std=c++11)
endif()

include_directories(include/)

find_package(OpenMP)
if (OPENMP_FOUND)
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

install(FILES ${INCLUDE_LIST} DESTINATION include/${PROJECT_NAME})
install(FILES ${CMakeConfig_LIST} DESTINATION lib/cmake/${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)


