cmake_minimum_required(VERSION 3.16)

project(QLink LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add my own libs
add_subdirectory(libEntity)
add_subdirectory(libMyWidget)

# Add executable 
add_executable(QLink
    main.cpp
    Game.cpp
)

# link the libs
target_link_libraries(QLink PUBLIC 
    Entity MyWidget
)
