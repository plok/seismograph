cmake_minimum_required(VERSION 3.10)
project(seismograph)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(modules/katla/external/skia/skia-cmake)
add_subdirectory(modules/katla/app-kit/)
add_subdirectory(modules/katla/gpu/)
add_subdirectory(modules/katla/gui-kit/)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

include(FindPkgConfig)
pkg_check_modules(GL REQUIRED gl)

find_package(glfw3 3.2.0 REQUIRED)
find_package(rxcpp REQUIRED)
find_package(portaudio REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR} externals/pitch-detection/lib externals/ffts/build)

set(sources
    main.cpp
    seismo-scene.cpp
    imgui-scene.cpp
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
    modules/katla/external/imgui
    ${PORTAUDIO_INCLUDE_DIR}
)

target_link_libraries(seismograph katla-guikit katla-appkit katla-gpu glfw ${GL_LIBRARIES} skia ${PORTAUDIO_LIBRARIES} ${Boost_LIBRARIES} pitch_detection ffts)
