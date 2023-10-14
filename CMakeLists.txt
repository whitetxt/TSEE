cmake_minimum_required(VERSION 3.14)
project(tsee)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)

###############################################################################
## file globbing ##############################################################
###############################################################################

# these instructions search the directory tree when CMake is
# invoked and put all files that match the pattern in the variables
# `sources`
file(GLOB_RECURSE sources src/*.cpp src/**/*.cpp src/*.hpp src/**/*.hpp)

message("sources: ${sources}")

# add the data to the target, so it becomes visible in some IDE
add_executable(tsee ${sources})

target_include_directories(tsee PRIVATE src/tsee/include src/tsee src)
# you can use set(sources src/main.cpp) etc if you don't want to
# use globbing to find files automatically

###############################################################################
## target definitions #########################################################
###############################################################################

# just for example add some compiler flags
target_compile_options(tsee PUBLIC -std=c++17 -Wall -Wextra -pedantic)

###############################################################################
## dependencies ###############################################################
###############################################################################

# Fetch fmt lib from git
FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG master
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(fmt)
target_link_libraries(tsee fmt::fmt)

FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.28.4
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL2)
target_link_libraries(tsee SDL2::SDL2main SDL2::SDL2-static)

FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.6.3
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

# START ADDITION
set(SDL2IMAGE_INSTALL OFF)
set(BUILD_SHARED_LIBS FALSE)
# END ADDITION

FetchContent_MakeAvailable(SDL2_image)
target_link_libraries(tsee SDL2_image::SDL2_image-static)

include_directories(tsee ${SDL2_INCLUDE_DIRS} ${_sdl2image_incdir})
target_link_libraries(tsee ${SDL2_LIBRARIES} ${_sdl2image_libdir})
