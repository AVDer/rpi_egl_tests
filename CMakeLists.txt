cmake_minimum_required(VERSION 3.0)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS "-O0 -g -std=c++14 -Wall -Wextra -Weffc++ -DRASPBERRY_PI -DOMX_SKIP64BIT")

file(GLOB SOURCE_FILES *.cpp)

#set(SOURCE_FILES ${SOURCES})


#main.cpp \
#egl_handler.cpp \
#dispmanx_handler.cpp \
#shader_program.cpp \
#omx_facade.cpp \
#omx_component.cpp \
#omx_port.cpp \
#omx_support.cpp\
#loggec.cpp)

project(eglTest)

add_executable(eglTest ${SOURCE_FILES})

target_link_libraries(eglTest EGL)
target_link_libraries(eglTest GLESv2)

target_link_libraries(eglTest bcm_host)
target_link_libraries(eglTest vchostif)
target_link_libraries(eglTest vcos)
target_link_libraries(eglTest vchiq_arm)
target_link_libraries(eglTest pthread)
target_link_libraries(eglTest openmaxil)