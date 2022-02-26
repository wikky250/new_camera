include(FetchContent)

FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG v1.9.2
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(spdlog)
