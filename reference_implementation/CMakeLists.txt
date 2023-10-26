cmake_minimum_required(VERSION 3.18)
project(smaug LANGUAGES C CXX) # CXX for google test

set(CMAKE_C_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(srcs
    src/dg.c
    src/pack.c
    src/poly.c
    src/key.c
    src/ciphertext.c
    src/hwt.c
    src/kem.c
    src/fips202.c
    src/io.c
    src/indcpa.c
)

if(MSVC)
  set(C_FLAGS /nologo /O2 /W4 /wd4146 /wd4244)
else()
  set(C_FLAGS -O3 -fomit-frame-pointer -fPIC -Wall -Wextra)
endif()

find_package(OpenSSL REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(RNG SHARED ${PROJECT_SOURCE_DIR}/src/rng.c)
target_compile_options(RNG PRIVATE -O3 -fomit-frame-pointer -fPIC)
target_link_libraries(RNG PRIVATE OpenSSL::Crypto)

set(LIB_NAME1 ${PROJECT_NAME}1)
add_library(${LIB_NAME1} SHARED ${srcs})
target_compile_definitions(${LIB_NAME1} PUBLIC SMAUG_MODE=1)
target_compile_options(${LIB_NAME1} PRIVATE ${C_FLAGS})
target_link_libraries(${LIB_NAME1} PUBLIC RNG)

set(LIB_NAME3 ${PROJECT_NAME}3)
add_library(${LIB_NAME3} SHARED ${srcs})
target_compile_definitions(${LIB_NAME3} PUBLIC SMAUG_MODE=3)
target_compile_options(${LIB_NAME3} PRIVATE ${C_FLAGS})
target_link_libraries(${LIB_NAME3} PUBLIC RNG)

set(LIB_NAME5 ${PROJECT_NAME}5)
add_library(${LIB_NAME5} SHARED ${srcs})
target_compile_definitions(${LIB_NAME5} PUBLIC SMAUG_MODE=5)
target_compile_options(${LIB_NAME5} PRIVATE ${C_FLAGS})
target_link_libraries(${LIB_NAME5} PUBLIC RNG)

add_subdirectory(test)
add_subdirectory(kat)
add_subdirectory(benchmark)