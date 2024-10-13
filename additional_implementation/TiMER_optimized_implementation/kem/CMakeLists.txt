cmake_minimum_required(VERSION 3.18)
project(TiMER LANGUAGES C CXX ASM) # CXX for google test

set(CMAKE_C_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE MATCHES Release)
  enable_testing()

  include(cmake/CPM.cmake)
  cpmaddpackage(
    NAME googletest
    GITHUB_REPOSITORY google/googletest
    GIT_TAG release-1.12.0
    VERSION 1.12.0
    OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt"
  )
endif(NOT CMAKE_BUILD_TYPE MATCHES Release)

set(SRCS
    ${PROJECT_SOURCE_DIR}/src/dg.c
    ${PROJECT_SOURCE_DIR}/src/pack.c
    ${PROJECT_SOURCE_DIR}/src/poly.c
    ${PROJECT_SOURCE_DIR}/src/key.c
    ${PROJECT_SOURCE_DIR}/src/ciphertext.c
    ${PROJECT_SOURCE_DIR}/src/hwt.c
    ${PROJECT_SOURCE_DIR}/src/kem.c
    ${PROJECT_SOURCE_DIR}/src/io.c
    ${PROJECT_SOURCE_DIR}/src/indcpa.c
    ${PROJECT_SOURCE_DIR}/src/hash.c
    ${PROJECT_SOURCE_DIR}/src/verify.c
    ${PROJECT_SOURCE_DIR}/src/cbd.c
)

set(SRCS_RNG
    ${PROJECT_SOURCE_DIR}/src/randombytes.c)

set(SRCS_FIPS202
    ${PROJECT_SOURCE_DIR}/src/keccak/KeccakP-1600-times4-SIMD256.c
    ${PROJECT_SOURCE_DIR}/src/keccak/KeccakP-1600-AVX2.S
    ${PROJECT_SOURCE_DIR}/src/fips202x4.c
    ${PROJECT_SOURCE_DIR}/src/fips202.c
)

set(SRCS_NTT
    ${PROJECT_SOURCE_DIR}/src/mul/basemul.S
    ${PROJECT_SOURCE_DIR}/src/mul/consts7681.c
    ${PROJECT_SOURCE_DIR}/src/mul/consts10753.c
    ${PROJECT_SOURCE_DIR}/src/mul/invntt.S
    ${PROJECT_SOURCE_DIR}/src/mul/ntt.S
    ${PROJECT_SOURCE_DIR}/src/mul/ntt.c
)


if(MSVC)
  set(C_FLAGS /nologo /O2 /W4 /wd4146 /wd4244)
else()
  set(C_FLAGS -O3 -fomit-frame-pointer -fPIC -Wall -Wextra -march=native -mtune=native -mavx2 -mbmi2 -mpopcnt)
endif()

find_package(OpenSSL REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/keccak)
include_directories(${PROJECT_SOURCE_DIR}/include/mul)

set(LIB_RNG RNG)
add_library(${LIB_RNG} SHARED ${SRCS_RNG})
target_compile_options(${LIB_RNG} PRIVATE -O3 -fomit-frame-pointer -fPIC -mavx2 -mbmi2 -mpopcnt)

set(LIB_FIPS202 FIPS202)
add_library(${LIB_FIPS202} SHARED ${SRCS_FIPS202})
target_compile_options(${LIB_FIPS202} PRIVATE -O3 -fomit-frame-pointer -fPIC -mavx2 -mbmi2 -mpopcnt)

option(ENABLE_CLANG_TIDY "Enable clang-tidy during build" OFF)
if(ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY_EXECUTABLE clang-tidy)

  if(CLANG_TIDY_EXECUTABLE)
    set(SOURCE_FILES_TO_CHECK ${SRCS})

    add_custom_target(clang-tidy
      COMMAND ${CLANG_TIDY_EXECUTABLE}
        -p ${CMAKE_BINARY_DIR}
        -header-filter=.*
        ${SOURCE_FILES_TO_CHECK}
      COMMENT "Running clang-tidy"
    )
  else()
    message(FATAL_ERROR "clang-tidy not found. Static analysis target will not be available.")
  endif()
endif()

option(ENABLE_CPPCHECK "Enable cppcheck during build" OFF)
if(ENABLE_CPPCHECK)
  find_program(CPPCHECK_EXECUTABLE cppcheck)

  if(CPPCHECK_EXECUTABLE)
    set(SOURCE_FILES_TO_CHECK ${srcs})
    set(CPPCHECK_OUTPUT_FILE "${CMAKE_BINARY_DIR}/cppcheck_results.xml") # Output file path

    add_custom_target(cppcheck
      COMMAND ${CPPCHECK_EXECUTABLE}
        --enable=warning,performance,portability,information
        --xml
        --output-file=${CPPCHECK_OUTPUT_FILE}
        -I ${CMAKE_SOURCE_DIR}/include
        ${SOURCE_FILES_TO_CHECK}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMENT "Running cppcheck"
    )
  else()
    message(FATAL_ERROR "cppcheck not found. Static analysis target will not be available.")
  endif()
endif()


set(SRCS ${SRCS} ${SRCS_NTT})

set(LIB_NAME1 ${PROJECT_NAME})
add_library(${LIB_NAME1} SHARED ${SRCS})
target_compile_definitions(${LIB_NAME1} PUBLIC TiMER_MODE=1)
target_compile_options(${LIB_NAME1} PRIVATE ${C_FLAGS})
target_link_libraries(${LIB_NAME1} PUBLIC ${LIB_RNG} ${LIB_FIPS202})


add_subdirectory(test)
add_subdirectory(kat)
add_subdirectory(benchmark)
