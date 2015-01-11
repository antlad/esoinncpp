cmake_minimum_required(VERSION 2.8)
project (esoinn)

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

add_library(esoinncpp ${LIB_TYPE} ${ESOINN_SRC} ${INCLUDE_LIST} ${CMakeConfig_LIST})

install(FILES ${INCLUDE_LIST} DESTINATION include/esoinncpp)
install(FILES ${CMakeConfig_LIST} DESTINATION lib/cmake/esoinncpp)

install(TARGETS esoinncpp
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)


