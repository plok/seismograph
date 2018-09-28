cmake_minimum_required(VERSION 3.10)
project(tuner)

set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)
pkg_check_modules(GL REQUIRED gl)
pkg_check_modules(LIBUV REQUIRED libuv)

find_package(glfw3 3.2.0 REQUIRED)

set (skia_path ../../../skia)
set (skia_output_path ${skia_path}/out/Shared/)
set (skia_include_base_path ${skia_path}/include)

find_library(skia skia ${skia_output_path})

set (skia_include_directories
    ${skia_include_base_path}
    ${skia_include_base_path}/core
    ${skia_include_base_path}/config
    ${skia_include_base_path}/gpu
    ${skia_include_base_path}/utils
    ${skia_include_base_path}/private
)

set(sources
    main.cpp
    event-loop-uv.cpp
    timer-uv.cpp
    signal-handler-uv.cpp
    ../../common/error.cpp
    ../../common/subject.cpp
    ../../renderer/drawing/skia-opengl.cpp

    ../../renderer/gpu/opengl/opengl.cpp
    ../../renderer/gpu/opengl/opengl-window.cpp
    ../../renderer/gpu/opengl/opengl-window-factory.cpp
)

add_executable(tuner ${sources})

target_include_directories(tuner PRIVATE
    .
    ..
    ../../
    ../../renderer/
    ../../external/
    ${skia_include_directories})

target_link_libraries(tuner glfw ${GL_LIBRARIES} ${LIBUV_LIBRARIES} ${skia})
