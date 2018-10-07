cmake_minimum_required(VERSION 3.10)
project(seismograph)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("modules/katla/app-kit/")
add_subdirectory("modules/katla/gpu/")
add_subdirectory("modules/katla/gui-kit/")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

include(FindPkgConfig)
pkg_check_modules(GL REQUIRED gl)

find_package(glfw3 3.2.0 REQUIRED)
find_package(rxcpp REQUIRED)
find_package(portaudio REQUIRED)

SET (skia_path ${PROJECT_SOURCE_DIR}/modules/katla/external/skia)
set (skia_include_base_path ${skia_path}/include)

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
    seismo-render-view.cpp
    audio/device-manager.cpp
)

add_executable(seismograph ${sources})

target_include_directories(seismograph PRIVATE
    .
    modules/
    modules/katla
    modules/katla/gui-kit/
    modules/katla/gpu/
    modules/katla/external/
    ${skia_include_directories}
    ${PORTAUDIO_INCLUDE_DIR}
)

target_link_libraries(seismograph katla-guikit katla-appkit katla-gpu glfw ${GL_LIBRARIES} ${skia} ${PORTAUDIO_LIBRARIES})
