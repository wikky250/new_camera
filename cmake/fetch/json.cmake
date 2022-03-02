include(FetchContent)

FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
    GIT_TAG v3.10.4
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(json)
