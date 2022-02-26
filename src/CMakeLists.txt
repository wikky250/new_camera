find_package(OpenCV REQUIRED)
include(fetch/spdlog)

if(CAMERA_HIK_SUPPORT)
  message(STATUS "src CAMERA_HIK_SUPPORT defined")
  add_subdirectory(hik)
  list(APPEND DEP_CAMERA_LIB hik_camera)
endif()

if(CAMERA_BALSER_SUPPORT)
  message(STATUS "src CAMERA_BALSER_SUPPORT defined")
  add_subdirectory(balser)
  list(APPEND DEP_CAMERA_LIB balser_camera)
endif()

if(CAMERA_DALSA_SUPPORT)
  message(STATUS "src CAMERA_DALSA_SUPPORT defined")
  add_subdirectory(dalsa)
  list(APPEND DEP_CAMERA_LIB dalsa_camera)
endif()

if(CAMERA_BAUMER_SUPPORT)
  message(STATUS "src CAMERA_BAUMER_SUPPORT defined")
  add_subdirectory(baumer)
  list(APPEND DEP_CAMERA_LIB baumer_camera)
endif()

if(CAMERA_DAHUA_SUPPORT)
  message(STATUS "src CAMERA_DAHUA_SUPPORT defined")
  add_subdirectory(dahua)
  list(APPEND DEP_CAMERA_LIB dahua_camera)
endif()

file(GLOB srcs *.cpp *.h)

add_library(camera_manager SHARED ${srcs})

target_include_directories(
  camera_manager PUBLIC ${OpenCV_INCLUDE_DIRS} spdlog::spdlog
                        ${PROJECT_SOURCE_DIR}/include ${THIRDPARTY_DIR} util)

target_link_libraries(camera_manager PRIVATE ${OpenCV_LIBS} spdlog::spdlog
                                             ${DEP_CAMERA_LIB})
