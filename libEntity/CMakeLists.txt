set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(Entity
    Entity.cpp
    Sprite.cpp
    Manager.cpp
    Map.cpp
    Animation.cpp 
    Hero.cpp
)

target_include_directories(Entity 
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
    PUBLIC "${PROJECT_BINARY_DIR}"
)

# Find the Qt's lib
list(APPEND CMAKE_PREFIX_PATH "D:/Qt/6.7.2/mingw_64/")
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

target_link_libraries(Entity PUBLIC
    PUBLIC Qt${QT_VERSION_MAJOR}::Widgets
)
