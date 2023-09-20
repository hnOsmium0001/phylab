cmake_minimum_required(VERSION 3.13)
project(phylab LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(glm REQUIRED)
find_package(fmt REQUIRED)
find_package(entt REQUIRED)

add_executable(${PROJECT_NAME}
    # add_executable requires at least one source file
    source/main.cpp
)
add_subdirectory(source)

target_include_directories(${PROJECT_NAME}
PRIVATE
    sources/
)
target_link_libraries(${PROJECT_NAME}
PRIVATE
    ${CONAN_LIBS}
)
