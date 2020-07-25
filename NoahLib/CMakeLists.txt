cmake_minimum_required(VERSION 3.9)

include_directories(
    "."
    "${THIRD_PARTY}/SDL/include"
    "${THIRD_PARTY}/imgui"
    "${THIRD_PARTY}/ImGuizmo"
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

 "${THIRD_PARTY}/ImGuizmo/ImGuizmo.cpp"
 "${THIRD_PARTY}/ImGuizmo/ImGuizmo.h"
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


macro(addShaderProgram vsname psname varyingname)

set(SOURCES
    ${SOURCES}
    ${THIRD_PARTY}/../data/shaders/${vsname}.sc
    ${THIRD_PARTY}/../data/shaders/${psname}.sc
    ${THIRD_PARTY}/../data/shaders/${varyingname}.sc
)

shaderc(FILE ${THIRD_PARTY}/../data/shaders/${vsname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${vsname}.glsl.bin VERTEX WINDOWS VARYINGDEF ${THIRD_PARTY}/../data/shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE 430)
shaderc(FILE ${THIRD_PARTY}/../data/shaders/${psname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${psname}.glsl.bin FRAGMENT WINDOWS VARYINGDEF ${THIRD_PARTY}/../data/shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE 430)

set(SOURCES
    ${SOURCES}
    ${THIRD_PARTY}/../data/shaders/generated/${vsname}.glsl.bin
    ${THIRD_PARTY}/../data/shaders/generated/${psname}.glsl.bin
)

IF(${CMAKE_SYSTEM_NAME} MATCHES iOS|tvOS)
    shaderc(FILE ${THIRD_PARTY}/../data/shaders/${vsname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${vsname}.metal.bin VERTEX IOS VARYINGDEF ${THIRD_PARTY}/../data/shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE metal)
    shaderc(FILE ${THIRD_PARTY}/../data/shaders/${psname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${psname}.metal.bin FRAGMENT IOS VARYINGDEF ${THIRD_PARTY}/../data/shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE metal)
    set(SOURCES
        ${SOURCES}
        ${THIRD_PARTY}/../data/shaders/generated/${vsname}.metal.bin
        ${THIRD_PARTY}/../data/shaders/generated/${psname}.metal.bin
    )
endif()

IF(${CMAKE_SYSTEM_NAME} MATCHES Darwin)
    shaderc(FILE ${THIRD_PARTY}/../data/shaders/${vsname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${vsname}.metal.bin VERTEX OSX VARYINGDEF ${THIRD_PARTY}/../data/shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE metal)
    shaderc(FILE ${THIRD_PARTY}/../data/shaders/${psname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${psname}.metal.bin FRAGMENT OSX VARYINGDEF ${THIRD_PARTY}/../data/shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE metal)
    set(SOURCES
        ${SOURCES}
        ${THIRD_PARTY}/../data/shaders/generated/${vsname}.metal.bin
        ${THIRD_PARTY}/../data/shaders/generated/${psname}.metal.bin
    )
endif()

IF(${CMAKE_SYSTEM_NAME} MATCHES Windows)
    shaderc(FILE ${THIRD_PARTY}/../data/shaders/${vsname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${vsname}.dx11.bin VERTEX WINDOWS VARYINGDEF ${THIRD_PARTY}/../data/shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE vs_5_0)
    shaderc(FILE ${THIRD_PARTY}/../data/shaders/${psname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${psname}.dx11.bin FRAGMENT WINDOWS VARYINGDEF ${THIRD_PARTY}/../data/shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE ps_5_0)
    shaderc(FILE ${THIRD_PARTY}/../data/shaders/${vsname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${vsname}.dx11.debug.bin DEBUG VERTEX WINDOWS VARYINGDEF ${THIRD_PARTY}/../data/shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE vs_5_0)
    shaderc(FILE ${THIRD_PARTY}/../data/shaders/${psname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${psname}.dx11.debug.bin DEBUG FRAGMENT WINDOWS VARYINGDEF ${THIRD_PARTY}/../data/shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE ps_5_0)
    set(SOURCES
        ${SOURCES}
        ${THIRD_PARTY}/../data/shaders/generated/${vsname}.dx11.bin
        ${THIRD_PARTY}/../data/shaders/generated/${psname}.dx11.bin
        ${THIRD_PARTY}/../data/shaders/generated/${vsname}.dx11.debug.bin
        ${THIRD_PARTY}/../data/shaders/generated/${psname}.dx11.debug.bin
    )

    
    shaderc(FILE ${THIRD_PARTY}/../data/shaders/${vsname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${vsname}.spirv.bin VERTEX LINUX VARYINGDEF ${THIRD_PARTY}/../data/shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE spirv)
    shaderc(FILE ${THIRD_PARTY}/../data/shaders/${psname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${psname}.spirv.bin FRAGMENT LINUX VARYINGDEF ${THIRD_PARTY}/../data/shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE spirv)
    set(SOURCES
        ${SOURCES}
        ${THIRD_PARTY}/../data/shaders/generated/${vsname}.spirv.bin
        ${THIRD_PARTY}/../data/shaders/generated/${psname}.spirv.bin
    )
endif()

endmacro()

addShaderProgram(vertexColor_vs vertexColor_ps vertexColor.varying.def)

add_library (NoahLib ${SOURCES})
