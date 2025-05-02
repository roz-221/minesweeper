cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Minesweeper)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)

add_executable(Minesweeper
        main.cpp
        WelcomeWindow.cpp
        Board.cpp
        Tile.cpp
        Leaderboard.cpp
        GameWindow.cpp)

target_compile_features(Minesweeper PRIVATE cxx_std_17)

target_link_libraries(Minesweeper
    PRIVATE
      SFML::Graphics
      SFML::Window
      SFML::System
)
