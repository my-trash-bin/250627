cmake_minimum_required(VERSION 3.10)
project(p250627 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(submodules/libcoro)

add_executable(${PROJECT_NAME} src/coroutine.cpp src/main.c)

target_link_libraries(${PROJECT_NAME} PRIVATE libcoro)
target_include_directories(${PROJECT_NAME} PRIVATE submodules/libcoro/include)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
