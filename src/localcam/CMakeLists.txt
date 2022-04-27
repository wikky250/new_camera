#file(GLOB_RECURSE srcs *.cpp *.h, impl/*.cpp, impl/*.h)

add_library(local_camera
SHARED
    ${srcs}
)

target_sources(local_camera
PUBLIC localcamera.h
PRIVATE localcamera.cpp
)
target_include_directories(local_camera
PUBLIC
    spdlog::spdlog
    ${PROJECT_SOURCE_DIR}/src/util
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(local_camera
PRIVATE
    spdlog::spdlog
    opencv_world
)
