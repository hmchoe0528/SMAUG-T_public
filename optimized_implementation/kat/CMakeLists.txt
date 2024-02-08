set(kat_srcs
    ${PROJECT_SOURCE_DIR}/kat/kat_kem.c
    ${PROJECT_SOURCE_DIR}/src/rng.c
)

find_package(OpenSSL REQUIRED)

# Rank-2 KEM
set(KAT_KEM_LIB_NAME1 ${PROJECT_NAME}1-kat-kem)
add_library(${KAT_KEM_LIB_NAME1} SHARED ${kat_srcs})
target_include_directories(${KAT_KEM_LIB_NAME1} PUBLIC ../include)
target_compile_definitions(${KAT_KEM_LIB_NAME1} PUBLIC CRYPTO_ALGNAME="SMAUG1_KEM")
target_compile_definitions(${KAT_KEM_LIB_NAME1} PUBLIC SMAUG_MODE=1)

add_executable(${KAT_KEM_LIB_NAME1}-main PQCgenKAT_kem.c)
target_link_libraries(${KAT_KEM_LIB_NAME1}-main ${KAT_KEM_LIB_NAME1} ${LIB_NAME1} OpenSSL::Crypto)

# Rank-3 KEM
set(KAT_KEM_LIB_NAME3 ${PROJECT_NAME}3-kat-kem)
add_library(${KAT_KEM_LIB_NAME3} SHARED ${kat_srcs})
target_include_directories(${KAT_KEM_LIB_NAME3} PUBLIC ../include)
target_compile_definitions(${KAT_KEM_LIB_NAME3} PUBLIC CRYPTO_ALGNAME="SMAUG3_KEM")
target_compile_definitions(${KAT_KEM_LIB_NAME3} PUBLIC SMAUG_MODE=3)

add_executable(${KAT_KEM_LIB_NAME3}-main PQCgenKAT_kem.c)
target_link_libraries(${KAT_KEM_LIB_NAME3}-main ${KAT_KEM_LIB_NAME3} ${LIB_NAME3} OpenSSL::Crypto)

# Rank-5 KEM
set(KAT_KEM_LIB_NAME5 ${PROJECT_NAME}5-kat-kem)
add_library(${KAT_KEM_LIB_NAME5} SHARED ${kat_srcs})
target_include_directories(${KAT_KEM_LIB_NAME5} PUBLIC ../include)
target_compile_definitions(${KAT_KEM_LIB_NAME5} PUBLIC CRYPTO_ALGNAME="SMAUG5_KEM")
target_compile_definitions(${KAT_KEM_LIB_NAME5} PUBLIC SMAUG_MODE=5)

add_executable(${KAT_KEM_LIB_NAME5}-main PQCgenKAT_kem.c)
target_link_libraries(${KAT_KEM_LIB_NAME5}-main ${KAT_KEM_LIB_NAME5} ${LIB_NAME5} OpenSSL::Crypto)


if(ENABLE_SYMMETRIC_90S)
    # Rank-2 KEM
    add_executable(${KAT_KEM_LIB_NAME1}-main-90s PQCgenKAT_kem.c)
    target_link_libraries(${KAT_KEM_LIB_NAME1}-main-90s ${KAT_KEM_LIB_NAME1} ${LIB_90S_NAME1} OpenSSL::Crypto)

    # Rank-3 KEM
    add_executable(${KAT_KEM_LIB_NAME3}-main-90s PQCgenKAT_kem.c)
    target_link_libraries(${KAT_KEM_LIB_NAME3}-main-90s ${KAT_KEM_LIB_NAME3} ${LIB_90S_NAME3} OpenSSL::Crypto)

    # Rank-5 KEM
    add_executable(${KAT_KEM_LIB_NAME5}-main-90s PQCgenKAT_kem.c)
    target_link_libraries(${KAT_KEM_LIB_NAME5}-main-90s ${KAT_KEM_LIB_NAME5} ${LIB_90S_NAME5} OpenSSL::Crypto)
endif()