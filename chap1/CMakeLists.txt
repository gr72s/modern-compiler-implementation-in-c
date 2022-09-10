cmake_minimum_required(VERSION 3.16)
project(chap1)

add_executable(${PROJECT_NAME} util.c slp.c prog1.c main.c tree.c)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})