cmake_minimum_required(VERSION 3.10)
project(split_images)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp)

find_package( OpenCV REQUIRED )
add_executable(split_images ${SOURCE_FILES})
include_directories(
  /usr/local/include/opencv
  /usr/local/include
  BEFORE
  /usr/include/opencv
)

link_directories(
  /usr/local/lib
  /usr/lib
)
#target_link_libraries(split_images /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4  stdc++fs )
target_link_libraries(split_images ${OpenCV_LIBS}  stdc++fs )
#target_link_libraries(split_images opencv_core opencv_imgproc opencv_imgcodec opencv_highgui opencv_imgproc stdc++fs )