set_languages("c++20")

add_requires("vcpkg::fmt", {alias = "fmt"})
add_requires("vcpkg::entt", {alias = "entt"})
add_requires("vcpkg::glfw3", {alias = "glfw3"})
add_requires("vcpkg::glm", {alias = "glm"})
add_requires("vcpkg::imgui[core,glfw-binding,opengl3-binding]", {alias = "imgui"})

target("phylab")
    set_kind("binary")
    add_files("source/*.cpp")
    add_packages("fmt", "entt", "glfw3", "glm", "imgui")
