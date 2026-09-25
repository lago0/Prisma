#[===[
cmake_minimum_required(VERSION 4.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Prisma)

#[===[
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
    src/ui/window/Inspector/Inspector.cpp

    lib/imgui_impl_sdlrenderer3.cpp
    lib/imgui_draw.cpp
    lib/imgui_tables.cpp
    lib/imgui_widgets.cpp
    lib/imgui.cpp
    lib/imgui_impl_sdl3.cpp
    lib/imgui_demo.cpp
)

file(GLOB_RECURSE SOURCE_FILES 
    "src/*.cpp"
    "src/*.hpp"
    "src/*.h"
)

add_executable(Prisma ${SOURCE_FILES})

target_include_directories(Prisma PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

find_package(SDL3 REQUIRED)

target_link_libraries(Prisma PRIVATE SDL3::SDL3)

]===]

cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Prisma)

find_package(SDL3 REQUIRED)

file(GLOB_RECURSE PRISMA_SOURCES 
    "src/*.cpp"
    "src/*.hpp"
    "src/*.h"
)

set(IMGUI_SOURCES
    lib/imgui.cpp
    lib/imgui_draw.cpp
    lib/imgui_tables.cpp
    lib/imgui_widgets.cpp
    lib/imgui_demo.cpp
    lib/imgui_impl_sdl3.cpp
    lib/imgui_impl_sdlrenderer3.cpp
)

add_executable(Prisma ${PRISMA_SOURCES} ${IMGUI_SOURCES})

target_include_directories(Prisma PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(Prisma PRIVATE SDL3::SDL3)
