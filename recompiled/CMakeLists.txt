cmake_minimum_required(VERSION 3.20)

project(recompiled)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# recompiled_rsp - Library containing the rsp recompiled functions
add_library(recompiled_rsp STATIC)

if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64")
    target_compile_options(recompiled_rsp PRIVATE
        -march=nehalem
    )
endif()

target_compile_options(recompiled_rsp PRIVATE
    -fno-strict-aliasing
    -ggdb
)

# Warnings
target_compile_options(recompiled_rsp PRIVATE
    -Wno-unused-variable
)

target_include_directories(recompiled_rsp PRIVATE
    ${PROJECT_SOURCE_DIR}/../lib/N64ModernRuntime/ultramodern/include
    ${PROJECT_SOURCE_DIR}/../lib/N64ModernRuntime/librecomp/include
)

file(GLOB RSP_C_SOURCES ${PROJECT_SOURCE_DIR}/rsp/*.c)
file(GLOB RSP_CXX_SOURCES ${PROJECT_SOURCE_DIR}/rsp/*.cpp)

target_sources(recompiled_rsp PRIVATE ${RSP_C_SOURCES} ${RSP_CXX_SOURCES})


# recompiled_funcs - Library containing the primary recompiler output
add_library(recompiled_funcs STATIC)

if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64")
    target_compile_options(recompiled_funcs PRIVATE
        -march=nehalem
    )
endif()

target_compile_options(recompiled_funcs PRIVATE
    -fno-strict-aliasing
    -ggdb
)

# Warnings
target_compile_options(recompiled_funcs PRIVATE
    -Wno-unused-variable
    -Wno-implicit-function-declaration
)

target_include_directories(recompiled_funcs PRIVATE
    ${PROJECT_SOURCE_DIR}/../lib/N64ModernRuntime/ultramodern/include
    ${PROJECT_SOURCE_DIR}/../lib/N64ModernRuntime/librecomp/include
)

file(GLOB FUNC_C_SOURCES ${PROJECT_SOURCE_DIR}/funcs/*.c)
file(GLOB FUNC_CXX_SOURCES ${PROJECT_SOURCE_DIR}/funcs/*.cpp)

target_sources(recompiled_funcs PRIVATE ${FUNC_C_SOURCES} ${FUNC_CXX_SOURCES})