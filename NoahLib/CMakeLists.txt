cmake_minimum_required(VERSION 3.9)

include_directories(
    "."
    "${THIRD_PARTY}/SDL/include"
    "${THIRD_PARTY}/imgui"
    "${THIRD_PARTY}/soloud/soloud/include"
    "${THIRD_PARTY}/bgfx.cmake/bgfx/include"
    "${THIRD_PARTY}/bgfx.cmake/bx/include"
    "${THIRD_PARTY}/bgfx.cmake/bgfx/3rdparty"
    "${THIRD_PARTY}/bgfx.cmake/bimg/include"
    "${THIRD_PARTY}/tinyxml2"
)

if(${MSVC})
    include_directories("${THIRD_PARTY}/bgfx.cmake/bx/include/compat/msvc")
endif()

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
 "${THIRD_PARTY}/imgui/imgui_tables.cpp"
 "${THIRD_PARTY}/imgui/imgui_widgets.cpp"
 "${THIRD_PARTY}/imgui/backends/imgui_impl_sdl.cpp"

 "${THIRD_PARTY}/tinyxml2/tinyxml2.cpp"
 "${THIRD_PARTY}/tinyxml2/tinyxml2.h"
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

macro(addShaderProgram vsname psname varyingname)

set(SOURCES
    ${SOURCES}
    shaders/${vsname}.sc
    shaders/${psname}.sc
    shaders/${varyingname}.sc
)

shaderc(FILE shaders/${vsname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${vsname}.glsl.bin VERTEX WINDOWS VARYINGDEF shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE 430)
shaderc(FILE shaders/${psname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${psname}.glsl.bin FRAGMENT WINDOWS VARYINGDEF shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE 430)

set(SOURCES
    ${SOURCES}
    ${THIRD_PARTY}/../data/shaders/generated/${vsname}.glsl.bin
    ${THIRD_PARTY}/../data/shaders/generated/${psname}.glsl.bin
)

IF(${CMAKE_SYSTEM_NAME} MATCHES iOS|tvOS)
    shaderc(FILE shaders/${vsname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${vsname}.metal.bin VERTEX IOS VARYINGDEF shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE metal)
    shaderc(FILE shaders/${psname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${psname}.metal.bin FRAGMENT IOS VARYINGDEF shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE metal)
    set(SOURCES
        ${SOURCES}
        ${THIRD_PARTY}/../data/shaders/generated/${vsname}.metal.bin
        ${THIRD_PARTY}/../data/shaders/generated/${psname}.metal.bin
    )
endif()

IF(${CMAKE_SYSTEM_NAME} MATCHES Darwin)
    shaderc(FILE shaders/${vsname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${vsname}.metal.bin VERTEX OSX VARYINGDEF shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE metal)
    shaderc(FILE shaders/${psname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${psname}.metal.bin FRAGMENT OSX VARYINGDEF shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE metal)
    set(SOURCES
        ${SOURCES}
        ${THIRD_PARTY}/../data/shaders/generated/${vsname}.metal.bin
        ${THIRD_PARTY}/../data/shaders/generated/${psname}.metal.bin
    )
endif()

IF(${CMAKE_SYSTEM_NAME} MATCHES Windows)
    shaderc(FILE shaders/${vsname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${vsname}.dx11.bin VERTEX WINDOWS VARYINGDEF shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE vs_5_0)
    shaderc(FILE shaders/${psname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${psname}.dx11.bin FRAGMENT WINDOWS VARYINGDEF shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE ps_5_0)
    shaderc(FILE shaders/${vsname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${vsname}.dx11.debug.bin DEBUG VERTEX WINDOWS VARYINGDEF shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE vs_5_0)
    shaderc(FILE shaders/${psname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${psname}.dx11.debug.bin DEBUG FRAGMENT WINDOWS VARYINGDEF shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE ps_5_0)
    set(SOURCES
        ${SOURCES}
        ${THIRD_PARTY}/../data/shaders/generated/${vsname}.dx11.bin
        ${THIRD_PARTY}/../data/shaders/generated/${psname}.dx11.bin
        ${THIRD_PARTY}/../data/shaders/generated/${vsname}.dx11.debug.bin
        ${THIRD_PARTY}/../data/shaders/generated/${psname}.dx11.debug.bin
    )

    
    shaderc(FILE shaders/${vsname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${vsname}.spirv.bin VERTEX LINUX VARYINGDEF shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE spirv)
    shaderc(FILE shaders/${psname}.sc OUTPUT ${THIRD_PARTY}/../data/shaders/generated/${psname}.spirv.bin FRAGMENT LINUX VARYINGDEF shaders/${varyingname}.sc INCLUDES ${BGFX_DIR}/src PROFILE spirv)
    set(SOURCES
        ${SOURCES}
        ${THIRD_PARTY}/../data/shaders/generated/${vsname}.spirv.bin
        ${THIRD_PARTY}/../data/shaders/generated/${psname}.spirv.bin
    )
endif()

endmacro()

add_bin2c("battleCursor.bin" BIN2C_DEPS)

set(SOURCES
    ${SOURCES}
    ${BIN2C_DEPS}
)

addShaderProgram(vertexColor_vs vertexColor_ps vertexColor.varying.def)
addShaderProgram(model3d_vs model3d_ps model3d.varying.def)
addShaderProgram(PSXGeneric_vs PSXGeneric_ps PSXGeneric.varying.def)

assign_source_group(${SOURCES})

add_library (NoahLib ${SOURCES} )

#if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#		target_compile_definitions( NoahLib PUBLIC "BX_CONFIG_DEBUG=1" )
#	else()
#		target_compile_definitions( NoahLib PUBLIC "BX_CONFIG_DEBUG=0" )
#endif()
target_compile_definitions( NoahLib PUBLIC "BX_CONFIG_DEBUG=0" )
target_compile_definitions( NoahLib PUBLIC "_CRT_SECURE_NO_WARNINGS" )

set_property(TARGET NoahLib PROPERTY CXX_STANDARD 20)
