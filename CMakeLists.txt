cmake_minimum_required(VERSION 3.16)

project(Blackjack LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# --- Core library: pure game logic, no console I/O ------------------------
# Everything here is unit-testable and reused by both the game and the tests.
add_library(blackjack_core STATIC
    Blackjack/Carte.cpp
    Blackjack/Lista.cpp
    Blackjack/Pachet.cpp
    Blackjack/Jucator.cpp
    Blackjack/Reguli.cpp
    Blackjack/Strategie.cpp
    Blackjack/Salvare.cpp
)
target_include_directories(blackjack_core PUBLIC Blackjack)

# --- Game executable: the console layer -----------------------------------
add_executable(blackjack
    Blackjack/Joc.cpp
    Blackjack/Source.cpp
)
target_link_libraries(blackjack PRIVATE blackjack_core)

if(NOT WIN32)
    find_package(Threads REQUIRED)
    target_link_libraries(blackjack PRIVATE Threads::Threads)
endif()

# --- Unit tests (Catch2 via FetchContent) ---------------------------------
option(BLACKJACK_BUILD_TESTS "Build the unit test suite" ON)

if(BLACKJACK_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.5.4
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(blackjack_tests
        tests/test_scor.cpp
        tests/test_pachet.cpp
        tests/test_reguli.cpp
        tests/test_strategie.cpp
        tests/test_lista.cpp
    )
    target_link_libraries(blackjack_tests PRIVATE blackjack_core Catch2::Catch2WithMain)

    enable_testing()
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(Catch)
    catch_discover_tests(blackjack_tests)
endif()
