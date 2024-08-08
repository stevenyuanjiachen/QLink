# build the lib
add_library(libGame Game.cpp)

# the lib's interface 
target_include_directories(libGame INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Entity"
)

# Find the Qt's lib
list(APPEND CMAKE_PREFIX_PATH "D:/Qt/6.7.2/mingw_64/")
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Add libEntity
add_subdirectory(Entity)

# Link libs 
target_link_libraries(libGame
    PUBLIC Qt${QT_VERSION_MAJOR}::Widgets
    PRIVATE libEntity
)