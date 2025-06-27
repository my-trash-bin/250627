cmake_minimum_required(VERSION 3.10)

project(p250627)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)

add_executable(${PROJECT_NAME} src/coroutine.cpp src/main.c)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
