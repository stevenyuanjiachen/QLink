add_library(Entity
    Manager.cpp
    Sprite.cpp
    Map.cpp
    PlayerPlane.cpp
    Bullet.cpp
    EnemyPlane.cpp
)

target_include_directories(Entity 
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Find the Qt's lib
list(APPEND CMAKE_PREFIX_PATH "D:/Qt/6.7.2/mingw_64/")
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Link libs
target_link_libraries(Entity PUBLIC
    PUBLIC Qt${QT_VERSION_MAJOR}::Widgets
)
