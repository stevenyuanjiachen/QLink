set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(MyWidget
    MyProgressBar.cpp
    ScoreBoard.cpp
    PauseMenu.cpp
)

target_include_directories(MyWidget
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Find the Qt's lib
list(APPEND CMAKE_PREFIX_PATH "D:/Qt/6.7.2/mingw_64/")
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

target_link_libraries(MyWidget PUBLIC
    PUBLIC Qt${QT_VERSION_MAJOR}::Widgets
)
