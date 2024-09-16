cmake_minimum_required(VERSION 3.16)

project(QLink LANGUAGES CXX)

set(CMAKE_EXE_LINKER_FLAGS "-static")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add my own libs
add_subdirectory(libEntity)
add_subdirectory(libMyWidget)
# Find the Qt's lib
list(APPEND CMAKE_PREFIX_PATH "D:/Qt/6.7.2/mingw_64/")
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

# Add src file
add_executable(QLink
    main.cpp
    Game.h
    Game.cpp
)

#Add include file
target_include_directories(QLink
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# link the libs
target_link_libraries(QLink PUBLIC 
    Entity MyWidget
    Qt${QT_VERSION_MAJOR}::Test
)
