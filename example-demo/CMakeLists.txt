include(fetch/OpenCV)
include(fetch/spdlog)

find_package(OpenCV)
find_package(Qt5 COMPONENTS Core Gui Widgets Network Sql REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


add_subdirectory(UIDemo)

if(DISABLE_DEBUG_INFO)
    add_executable(CameraTest WIN32)
else()
    add_executable(CameraTest)
endif()

target_sources(CameraTest
PRIVATE
    main.cpp        
)
target_link_libraries(CameraTest
PRIVATE
    uidemo
    ${OpenCV_LIBS}
    Qt5::Widgets
    Qt5::Core
    spdlog::spdlog
)
target_include_directories(CameraTest
PUBLIC
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
    spdlog::spdlog
    ${PROJECT_SOURCE_DIR}/src/util
)


add_custom_command(
    TARGET CameraTest
    POST_BUILD
    COMMAND "$<TARGET_FILE_DIR:Qt5::qmake>/windeployqt.exe" "--no-translations" $<TARGET_FILE:CameraTest>
    WORKING_DIRECTORY $<TARGET_FILE_DIR:CameraTest>
)

add_custom_command(TARGET CameraTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:CameraTest> $<TARGET_FILE_DIR:CameraTest>
    COMMAND_EXPAND_LISTS
)