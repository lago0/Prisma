cmake_minimum_required(VERSION 4.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(ImageEditor)

add_executable(ImageEditor
    src/main.cpp
    src/core/Core.cpp
    src/layer/LayerManager.cpp
    src/layer/Layer.cpp
    src/node/Node.cpp
    src/node/NodeInput.cpp
    src/node/NodeWorkspace.cpp

    src/ui/UI.cpp
    src/ui/Window.cpp
    src/ui/window/LayerWindow/LayerWindow.cpp

    lib/imgui_impl_sdlrenderer3.cpp
    lib/imgui_draw.cpp
    lib/imgui_tables.cpp
    lib/imgui_widgets.cpp
    lib/imgui.cpp
    lib/imgui_impl_sdl3.cpp
    lib/imgui_demo.cpp
)

target_include_directories(ImageEditor PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

find_package(SDL3 REQUIRED)

target_link_libraries(ImageEditor PRIVATE SDL3::SDL3)