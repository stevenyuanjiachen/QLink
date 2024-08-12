cmake_minimum_required(VERSION 3.16)

project(xiaoxiaole LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add my own libs
add_subdirectory(libEntity)

# Add executable 
add_executable(xiaoxiaole
  main.cpp
  Game.cpp
)

# link the libs
target_link_libraries(xiaoxiaole 
  Entity  
)

# install settings
include(GNUInstallDirs)
install(TARGETS xiaoxiaole
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
