cmake_minimum_required(VERSION 3.21)

project(ImgProcess_in_C C)

set(CMAKE_C_STANDARD 17)

aux_source_directory(processAPI P)
aux_source_directory(fileIO IO)
aux_source_directory(settings ST)
aux_source_directory(base BS)

add_executable(ImgProcess_in_C main.c header.h ${P} ${IO} ${ST} ${BS} fileIO/jpegIO.h)

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CURRENT_SYSTEM Linux)
    target_link_libraries(ImgProcess_in_C m)
    #[[target_link_libraries(ImgProcess_in_C pthread)]]

ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CURRENT_SYSTEM Windows)
ENDIF()