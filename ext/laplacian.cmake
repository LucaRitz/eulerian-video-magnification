include(FetchContent)

option(LAPLACIAN_PYRAMID_BUILD_TEST "" OFF)
option(LAPLACIAN_PYRAMID_BUILD_DOCUMENTATION "" OFF)
option(LAPLACIAN_PYRAMID_BUILD_CODE_DOCUMENTATION "" OFF)
option(LAPLACIAN_PYRAMID_BUILD_EXT_LIBS "" OFF)

FetchContent_Declare(
        laplacian_pyramid
        GIT_REPOSITORY https://github.com/LucaRitz/laplacian-pyramid
        GIT_TAG        main
)
FetchContent_MakeAvailable(laplacian_pyramid)

unset(LAPLACIAN_PYRAMID_BUILD_TEST)
unset(LAPLACIAN_PYRAMID_BUILD_DOCUMENTATION)
unset(LAPLACIAN_PYRAMID_BUILD_CODE_DOCUMENTATION)
unset(LAPLACIAN_PYRAMID_BUILD_EXT_LIBS)