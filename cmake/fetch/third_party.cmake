if(NOT DEFINED 3rdparty_URL)
        set(THIRDPARTY_URL "http://artifactory.sm/artifactory/cameraservice/20210915/third_party0915.7z")
        set(THIRDPARTY_MD5 "a67786d136a95788fba8b32b23fbb86b")
endif()

include(FetchContent)

FetchContent_Declare(THIRDPARTY
    URL ${THIRDPARTY_URL}
    URL_MD5 ${THIRDPARTY_MD5}
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/third_party
)

message(STATUS "Fetch THIRDPARTY")
FetchContent_MakeAvailable(THIRDPARTY)
message(STATUS "Fetch THIRDPARTY done")

FetchContent_GetProperties(THIRDPARTY SOURCE_DIR THIRDPARTY_DIR)
