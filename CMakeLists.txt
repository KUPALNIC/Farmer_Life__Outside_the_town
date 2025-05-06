cmake_minimum_required(VERSION 3.29)
project(Farmer_Life__Outside_the_town)

set(CMAKE_CXX_STANDARD 17)
set(SFML_INCLUDE_DIR "/usr/include")
set(SFML_LIBRARY_DIR "/usr/lib")

include_directories(${SFML_INCLUDE_DIR})
link_directories(${SFML_LIBRARY_DIR})

add_executable(Farmer_Life__Outside_the_town
        src/main.cpp
        src/Game/Game.hpp
        src/Game/Game.cpp
        src/Game/Player.cpp
        src/Game/Player.hpp
        src/Game/Camera.cpp
        src/Game/Camera.hpp
        src/Game/World.cpp
        src/Game/World.hpp
        src/Objects/Crop.cpp
        src/Objects/Crop.hpp
        src/Objects/Bed.hpp
        src/Objects/Bed.cpp
        src/Objects/Tree.cpp
        src/Objects/Tree.hpp
        src/Objects/Tool.cpp
        src/Objects/Tool.hpp)

find_package(SFML 3.0 COMPONENTS System Window Graphics REQUIRED)
target_link_libraries(Farmer_Life__Outside_the_town sfml-graphics sfml-window sfml-system)