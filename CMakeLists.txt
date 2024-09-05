cmake_minimum_required(VERSION 3.16)

project(QLink LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Config file
# provide the Source Path
configure_file(xiaoxiaoleConfig.h.in xiaoxiaoleConfig.h)
set(RESOURCE_DIR "${PROJECT_SOURCE_DIR}/images")

# Add my own libs
add_subdirectory(libEntity)

# Add executable 
add_executable(QLink
  main.cpp
  Game.cpp
)

# include the config file
target_include_directories(xiaoxiaole
  PUBLIC "${PROJECT_BINARY_DIR}"
)

# link the libs
<<<<<<< HEAD
target_link_libraries(QLink 
  Entity  
=======
target_link_libraries(xiaoxiaole 
  PUBLIC Entity
>>>>>>> 5909b48 (Add the ConfigFile)
)
