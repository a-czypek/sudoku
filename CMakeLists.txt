cmake_minimum_required(VERSION 3.20)
project(sudoku C)

add_executable(sudoku main.c)
target_link_libraries(sudoku m)
set(CMAKE_EXE_LINKER_FLAGS "-mconsole")
