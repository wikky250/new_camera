file(GLOB_RECURSE srcs *.cpp *.h, impl/*.cpp, impl/*.h)

add_library(balser_camera
SHARED
    ${srcs}
)

target_include_directories(balser_camera
PUBLIC
    ${OpenCV_INCLUDE_DIRS}
    spdlog::spdlog
    ${PROJECT_SOURCE_DIR}/src/util
    ${PROJECT_SOURCE_DIR}/include
    ${THIRDPARTY_DIR}
    ${THIRDPARTY_DIR}/pylon6/include
)

target_link_libraries(balser_camera
PRIVATE
    ${OpenCV_LIBS}
    spdlog::spdlog
    ${THIRDPARTY_DIR}/pylon6/lib/x64/GCBase_MD_VC141_v3_1_Basler_pylon.lib
    ${THIRDPARTY_DIR}/pylon6/lib/x64/GenApi_MD_VC141_v3_1_Basler_pylon.lib
    ${THIRDPARTY_DIR}/pylon6/lib/x64/PylonUtility_v6_2.lib
    ${THIRDPARTY_DIR}/pylon6/lib/x64/PylonBase_v6_2.lib
)
