cmake_minimum_required(VERSION 3.15)
project(camera-test_application VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CAMERA_HIK_SUPPORT "Support Hik SDK" OFF)
option(CAMERA_LOCAL_SUPPORT "Support Local SDK" ON)
option(CAMERA_BALSER_SUPPORT "Support Balser SDK" OFF)
option(CAMERA_DALSA_SUPPORT "Support Dalsa SDK"   OFF)
option(CAMERA_BAUMER_SUPPORT "Support Baumer SDK"   OFF)
option(CAMERA_DAHUA_SUPPORT "Support Dahua SDK"   OFF)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
# 打开以下set指令，可以指定本地三方依赖库地址，跳过从网络下载
#set (THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/third_party)
if(NOT DEFINED THIRDPARTY_DIR)
    include(fetch/third_party)
endif()

set(OpenCV_DIR ${THIRDPARTY_DIR}/opencv/x64/vc15/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_compile_options(
    $<$<AND:$<NOT:$<COMPILE_LANGUAGE:CUDA>>,$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>>:/utf-8>
)

add_subdirectory(src)
add_subdirectory(example-demo)