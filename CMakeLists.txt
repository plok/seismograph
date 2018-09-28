cmake_minimum_required(VERSION 3.10)
project(seismograph)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("modules/katla/app-kit/")
add_subdirectory("modules/katla/renderer/")

include(FindPkgConfig)
pkg_check_modules(GL REQUIRED gl)

find_package(glfw3 3.2.0 REQUIRED)

SET (skia_path $ENV{SKIA_PATH})

if (NOT DEFINED skia_path)
    SET(skia_path ${skia_env_path})
endif()

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
)

add_executable(seismograph ${sources})

target_include_directories(seismograph PRIVATE
    .
    modules/katla
    modules/katla/renderer/
    modules/katla/external/
    ${skia_include_directories}
)

target_link_libraries(seismograph katla-appkit katla-renderer glfw ${GL_LIBRARIES} ${skia})
