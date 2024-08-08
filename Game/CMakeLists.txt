# build the lib
add_library(libGame Game.cpp)

# the lib's interface 
target_include_directories(libGame INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Find the Qt's lib
list(APPEND CMAKE_PREFIX_PATH 
  "D:/Qt/6.7.2/mingw_64/"
)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# add Widgets
target_link_libraries(libGame
    Qt${QT_VERSION_MAJOR}::Widgets
)