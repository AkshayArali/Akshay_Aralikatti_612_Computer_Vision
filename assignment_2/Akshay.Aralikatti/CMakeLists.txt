cmake_minimum_required(VERSION 3.10)
project(sobel)

# Set C++ standard 
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenCV package
find_package(OpenCV REQUIRED)

#create an executable target with your source file(s)
add_executable(sobel sobel.cpp)

#Link the executable with OpenCV libraries
target_link_libraries(sobel PRIVATE ${OpenCV_LIBS})

# Optionally, include OpneCV headers for Intellisense or compile time
target_include_directories(sobel PRIVATE ${OpenCV_INCLUDE_DIRS})