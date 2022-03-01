#file(GLOB_RECURSE srcs *.cpp *.h, impl/*.cpp, impl/*.h)

add_library(basler_camera
SHARED
    ${srcs}
)

target_sources(basler_camera
PUBLIC baslercamera.h
PRIVATE baslercamera.cpp
)
target_include_directories(basler_camera
PUBLIC
    spdlog::spdlog
    ${PROJECT_SOURCE_DIR}/src/util
    ${PROJECT_SOURCE_DIR}/include
    ${THIRDPARTY_DIR}
    ${THIRDPARTY_DIR}/pylon6/include
)

target_link_libraries(basler_camera
PRIVATE
    spdlog::spdlog
    ${THIRDPARTY_DIR}/pylon6/lib/x64/GCBase_MD_VC141_v3_1_Basler_pylon.lib
    ${THIRDPARTY_DIR}/pylon6/lib/x64/GenApi_MD_VC141_v3_1_Basler_pylon.lib
    ${THIRDPARTY_DIR}/pylon6/lib/x64/PylonUtility_v6_2.lib
    ${THIRDPARTY_DIR}/pylon6/lib/x64/PylonBase_v6_2.lib
)
