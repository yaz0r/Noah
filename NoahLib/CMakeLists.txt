cmake_minimum_required(VERSION 3.9)

include_directories(
    "."
    "${THIRD_PARTY}/SDL/include"
    "${THIRD_PARTY}/imgui"
    "${THIRD_PARTY}/soloud/soloud/include"
#    "${THIRD_PARTY}/glm"
#    "${THIRD_PARTY}/tracy"
    "${THIRD_PARTY}/bgfx.cmake/bgfx/include"
    "${THIRD_PARTY}/bgfx.cmake/bx/include"
    "${THIRD_PARTY}/bgfx.cmake/bx/include/compat/msvc"
    "${THIRD_PARTY}/bgfx.cmake/bgfx/3rdparty"
    "${THIRD_PARTY}/bgfx.cmake/bimg/include"
#    "${THIRD_PARTY}/aosdk"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SRC_ROOT
    "*.cpp"
    "*.h"
)

set(SOURCES
 ${SOURCES}
 ${SRC_ROOT}
 "${THIRD_PARTY}/imgui/imgui.cpp"
 "${THIRD_PARTY}/imgui/imgui.h"
 "${THIRD_PARTY}/imgui/imgui_draw.cpp"
 "${THIRD_PARTY}/imgui/imgui_widgets.cpp"
 "${THIRD_PARTY}/imgui/examples/imgui_impl_sdl.cpp"
)

function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

assign_source_group(${SOURCES})

add_library (NoahLib ${SOURCES})
