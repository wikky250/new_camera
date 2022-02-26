if(NOT DEFINED OpenCV_URL)
    if(WIN32)
        set(OpenCV_URL "http://artifactory.sm/artifactory/opencv/opencv-4.2.0-windows.zip")
        set(OpenCV_MD5 "41387503cbf07a706a28e629459e575b")
    else()
        set(OpenCV_URL "http://artifactory.sm/artifactory/opencv/opencv-4.2.0-linux-x64.zip")
        set(OpenCV_MD5 "80aa2bd4066ef1784505ce35486551c3")
    endif()
endif()


include(FetchContent)

FetchContent_Declare(OpenCV
    URL ${OpenCV_URL}
    URL_MD5 ${OpenCV_MD5}
)

FetchContent_MakeAvailable(OpenCV)

FetchContent_GetProperties(OpenCV SOURCE_DIR OpenCV_SOURCE_DIR)

# get OpenCV_DIR
if(WIN32)
    set(OpenCV_DIR ${OpenCV_SOURCE_DIR}/x64/vc15/lib)
else()
    set(OpenCV_DIR ${OpenCV_SOURCE_DIR}/lib/cmake/opencv4)
endif()
