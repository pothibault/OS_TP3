cmake_minimum_required(VERSION 3.22)
project(TP3_OS)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)

add_executable(TP3_OS 
    main.cpp 
    FileSystem.cpp 
    BlockDevice.cpp
)


