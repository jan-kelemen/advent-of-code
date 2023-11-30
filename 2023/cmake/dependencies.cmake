include(FetchContent)
FetchContent_Declare(
    lexy
    GIT_REPOSITORY https://github.com/foonathan/lexy.git
    GIT_TAG 7e583fe0c717715a10227d2d4ba14581143cf0af
)
FetchContent_MakeAvailable(lexy)

find_package(Boost REQUIRED)
