cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

project(pantiltcam-dlink)

find_package(CURL REQUIRED)
find_package(OpenCV REQUIRED)
message("OPENCV lib = ${OpenCV_LIBS} CURL INCLUDE = ${OpenCV_INCLUDE_DIR}")
message("CURL lib = ${CURL_LIBRARY} CURL INCLUDE = ${CURL_INCLUDE_DIR}")
include_directories(${OpenCV_INCLUDE_DIR} ${CURL_INCLUDE_DIRS})
add_executable(test-dlink   src/camera.cpp
                            src/visServoing/Castan.cpp
                            src/visServoing/Gauss.cpp
                            src/visServoing/Castan.h
                            src/visServoing/Gauss.h
                            src/visServoing/Definitions.h)


target_link_libraries(test-dlink ${OpenCV_LIBS} ${CURL_LIBRARY})


