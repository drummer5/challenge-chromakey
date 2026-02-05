cmake_minimum_required(VERSION 3.16)
project(ChromaKeyStudio VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Automatic MOC (required for Qt Signals/Slots)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 1. Find Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)

# 2. Find OpenCV
find_package(OpenCV REQUIRED)

# 3. Define the Executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/MainWindow.cpp
    src/VideoProcessor.cpp
    include/MainWindow.hpp
    include/VideoProcessor.hpp
)

# 4. Link Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Widgets
    ${OpenCV_LIBS}
)

# 5. Include Directories
target_include_directories(${PROJECT_NAME} PRIVATE include)