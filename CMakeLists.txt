cmake_minimum_required(VERSION 3.23)
project(renderer LANGUAGES C)

add_library(glad OBJECT "external/glad/glad.c")
target_include_directories(glad PUBLIC "external/glad/")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.h" "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})
unset(SOURCES)

find_package(glfw3 3.3 REQUIRED)
find_package(libmonotime REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad libmonotime::monotime)
