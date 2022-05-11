project(Noah)
cmake_minimum_required(VERSION 3.9)

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

set(THIRD_PARTY "${CMAKE_SOURCE_DIR}/ThirdParty")

set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BGFX_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory( ${THIRD_PARTY}/bgfx.cmake )

add_subdirectory( ${THIRD_PARTY}/soloud.cmake )

set(HAVE_LIBC ON CACHE BOOL "" FORCE)
add_subdirectory( ${THIRD_PARTY}/SDL )

add_subdirectory( NoahLib )
add_subdirectory( Noah )