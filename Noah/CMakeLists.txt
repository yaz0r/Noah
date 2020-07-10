cmake_minimum_required(VERSION 3.9)

include_directories(
    "."
    "${CMAKE_SOURCE_DIR}/NoahLib"
    "${THIRD_PARTY}"
    "${THIRD_PARTY}/SDL/include"
    "${THIRD_PARTY}/soloud/soloud/include"
    "${THIRD_PARTY}/imgui"
    "${THIRD_PARTY}/bgfx.cmake/bgfx/include"
#    "${THIRD_PARTY}/glm"
#    "${THIRD_PARTY}/tracy"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB PDS_DATA
    "${CMAKE_SOURCE_DIR}/data/*"
)

file(GLOB SRC_ROOT
    "*.cpp"
    "*.h"
)

file(GLOB_RECURSE SRC_KERNEL
    "kernel/*.cpp"
    "kernel/*.h"
)

file(GLOB_RECURSE SRC_FIELD_A3
    "field/field_a3/*.cpp"
    "field/field_a3/*.h"
)

set(SOURCES
 ${SOURCES}
 ${SRC_KERNEL}
 ${SRC_ROOT}
 ${SRC_FIELD_A3}
)

source_group("src" FILES ${SRC_ROOT})
source_group("src\\kernel" FILES ${SRC_KERNEL})
source_group("src\\field\\field_a3" FILES ${SRC_FIELD_A3})

set( CMAKE_EXE_LINKER_FLAGS_SHIPPING "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} /LTCG /OPT:ICF /OPT:REF /SUBSYSTEM:WINDOWS /DEBUG" )

add_executable (Noah ${SOURCES})

TARGET_LINK_LIBRARIES(Noah SDL2main SDL2-static soloud NoahLib bgfx ${EXTRA_LIBS})

set_target_properties(Noah PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/data")
set_target_properties(Noah PROPERTIES WIN32_EXECUTABLE FALSE)

set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT Noah)



