cmake_minimum_required(VERSION 4.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Prisma)

add_executable(Prisma
    src/main.cpp
    src/core/Core.cpp
    src/layer/LayerManager.cpp
    src/layer/Layer.cpp
    src/node/Node.cpp
    src/node/NodeInput.cpp
    src/node/NodeWorkspace.cpp
    src/compositor/Compositor.cpp

    src/ui/UI.cpp
    src/ui/Window.cpp
    src/ui/window/LayerWindow/LayerWindow.cpp
    src/ui/window/NodeViewer/NodeViewer.cpp

    lib/imgui_impl_sdlrenderer3.cpp
    lib/imgui_draw.cpp
    lib/imgui_tables.cpp
    lib/imgui_widgets.cpp
    lib/imgui.cpp
    lib/imgui_impl_sdl3.cpp
    lib/imgui_demo.cpp
)

target_include_directories(Prisma PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

find_package(SDL3 REQUIRED)

target_link_libraries(Prisma PRIVATE SDL3::SDL3)