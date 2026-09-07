cmake_minimum_required(VERSION 3.16)

project(Blackjack LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

file(GLOB BLACKJACK_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Blackjack/*.cpp")

add_executable(blackjack ${BLACKJACK_SOURCES})

if(NOT WIN32)
    find_package(Threads REQUIRED)
    target_link_libraries(blackjack PRIVATE Threads::Threads)
endif()
