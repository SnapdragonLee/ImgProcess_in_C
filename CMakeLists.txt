cmake_minimum_required(VERSION 3.21)

project(ImgProcess_in_C C)

set(CMAKE_C_STANDARD 17)

add_executable(ImgProcess_in_C main.c ProcessAPI.h bmpFile_IO.h ProcessAPI.c)

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CURRENT_SYSTEM Linux)
    target_link_libraries(ImgProcess_in_C m)
    #[[target_link_libraries(ImgProcess_in_C pthread)]]

ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CURRENT_SYSTEM Windows)
ENDIF()