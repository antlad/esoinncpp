project(tests)
cmake_minimum_required(VERSION 2.8)

set(KAGGLE_LIST
main.cpp
kaggleparser.h
kaggleparser.cpp
filter.h
filter.cpp

)

set(OCI_SRC_LIST
ocidataset.h
ocidataset.cpp
ocimain.cpp
)



add_definitions(-DBUILD_WITH_PNG_EXPORT_SUPPORT)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS REQUIRED filesystem system)
find_package(esoinncpp REQUIRED)
find_package(PNG REQUIRED)

if (NOT MSVC)
add_definitions(-std=c++11)
endif()

add_executable(kagglediggits ${KAGGLE_LIST})
add_executable(ocitest ${OCI_SRC_LIST})

include_directories( ${esoinncpp_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
target_link_libraries(kagglediggits ${esoinncpp_LIBRARIES} ${Boost_LIBRARIES} ${PNG_LIBRARY})

