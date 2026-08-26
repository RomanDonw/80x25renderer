cmake_minimum_required(VERSION 3.23)

# ========== SETTINGS ==========
project(tmrenderer VERSION 0.0 LANGUAGES C)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
# ========== ======== ==========

add_library(glad OBJECT "external/glad/glad.c")
target_include_directories(glad PUBLIC "external/glad/")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.h" "src/*.c")
add_library(${PROJECT_NAME} ${SOURCES})
unset(SOURCES)

target_include_directories(${PROJECT_NAME} PUBLIC "include/")

find_package(glfw3 3.3 REQUIRED)
find_package(libmonotime REQUIRED)
find_package(libncore REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw libmonotime::monotime libncore::ncore glad)

# ==============================

add_executable(test0 "tests/0.c")
target_link_libraries(test0 PRIVATE ${PROJECT_NAME})