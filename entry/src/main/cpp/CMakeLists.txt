# the minimum version of CMake.
cmake_minimum_required(VERSION 3.5.0)
project(NativeCPP)

set(NATIVERENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

if(DEFINED PACKAGE_FIND_FILE)
    include(${PACKAGE_FIND_FILE})
endif()
set(MODULE_NAME calc)
set(MODULE_NAME2 ugc)
add_definitions(-D_UNICODE)
add_definitions(-DUNICODE)
add_definitions(-D_UGUNICODE)

include_directories(${NATIVERENDER_ROOT_PATH}
                    ${NATIVERENDER_ROOT_PATH}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/../include/)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/arm64-v8a)

add_library(${MODULE_NAME} SHARED napi_init.cpp)
add_library(${MODULE_NAME2} SHARED napi_ugc_init.cpp)
target_link_libraries(${MODULE_NAME} PUBLIC libace_napi.z.so
                        PUBLIC libhilog_ndk.z.so)
target_link_libraries(${MODULE_NAME2} PUBLIC libace_napi.z.so
                        PUBLIC libhilog_ndk.z.so)