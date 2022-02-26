add_library(uidemo INTERFACE)

target_sources(uidemo
PUBLIC
	QtCameraTest.h
	QtCameraTest.cpp
	QtCameraTest.ui
)

target_link_libraries(uidemo
INTERFACE
	Qt5::Widgets
    Qt5::Core
)
target_include_directories(uidemo
INTERFACE ${PROJECT_SOURCE_DIR}/src
INTERFACE ./
)