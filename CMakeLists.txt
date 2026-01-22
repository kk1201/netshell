cmake_minimum_required(VERSION 3.19)
project(netshell)

set(CMAKE_C_STANDARD 23)
add_executable("${CMAKE_PROJECT_NAME}")

include_directories("${CMAKE_SOURCE_DIR}" /include)
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE
	src/main.c
)
