cmake_minimum_required(VERSION 2.8)
PROJECT(gltest)
SET(SRC_LIST fbo_example.cpp glslprogram.cpp)
INCLUDE_DIRECTORIES(/usr/include/)
LINK_DIRECTORIES("/usr/local/lib/"
"/usr/lib/" "/root/opencv/opencv-3.1.0/build/3rdparty/lib/")
link_libraries(GL GLU glut GLEW opencv_core opencv_imgproc opencv_imgcodecs
jasper tiff zlib IlmImf drm rt)
ADD_EXECUTABLE(opengl_test ${SRC_LIST})
#target_link_libraries(opengl_test opencv_core opencv_imgproc)
