cmake_minimum_required(VERSION 3.14)
project(tsee)

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
# FetchContent_Declare(fmt
#   GIT_REPOSITORY https://github.com/fmtlib/fmt.git
#   GIT_TAG master
# )
# FetchContent_MakeAvailable(fmt)
add_subdirectory(src/tsee/ext/fmt)
target_link_libraries(tsee fmt::fmt)

find_package(SDL2 REQUIRED)
include_directories(tsee ${SDL2_INCLUDE_DIRS})
target_link_libraries(tsee ${SDL2_LIBRARIES})
