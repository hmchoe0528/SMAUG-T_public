set(kat_srcs
    ${PROJECT_SOURCE_DIR}/kat/kat_kem.c
    ${PROJECT_SOURCE_DIR}/src/rng.c
)

find_package(OpenSSL REQUIRED)

# Rank-2 KEM
set(KAT_KEM_LIB_NAME1 ${PROJECT_NAME}1-kat-kem)
add_library(${KAT_KEM_LIB_NAME1} SHARED ${kat_srcs})
target_include_directories(${KAT_KEM_LIB_NAME1} PUBLIC ../include)
target_compile_definitions(${KAT_KEM_LIB_NAME1} PUBLIC CRYPTO_ALGNAME="SMAUG-T1_KEM")
target_compile_definitions(${KAT_KEM_LIB_NAME1} PUBLIC SMAUG_MODE=1)

add_executable(${LIB_NAME1}-kat PQCgenKAT_kem.c)
target_link_libraries(${LIB_NAME1}-kat ${KAT_KEM_LIB_NAME1} ${LIB_NAME1} OpenSSL::Crypto)

# Rank-3 KEM
set(KAT_KEM_LIB_NAME3 ${PROJECT_NAME}3-kat-kem)
add_library(${KAT_KEM_LIB_NAME3} SHARED ${kat_srcs})
target_include_directories(${KAT_KEM_LIB_NAME3} PUBLIC ../include)
target_compile_definitions(${KAT_KEM_LIB_NAME3} PUBLIC CRYPTO_ALGNAME="SMAUG-T3_KEM")
target_compile_definitions(${KAT_KEM_LIB_NAME3} PUBLIC SMAUG_MODE=3)

add_executable(${LIB_NAME3}-kat PQCgenKAT_kem.c)
target_link_libraries(${LIB_NAME3}-kat ${KAT_KEM_LIB_NAME3} ${LIB_NAME3} OpenSSL::Crypto)

# Rank-5 KEM
set(KAT_KEM_LIB_NAME5 ${PROJECT_NAME}5-kat-kem)
add_library(${KAT_KEM_LIB_NAME5} SHARED ${kat_srcs})
target_include_directories(${KAT_KEM_LIB_NAME5} PUBLIC ../include)
target_compile_definitions(${KAT_KEM_LIB_NAME5} PUBLIC CRYPTO_ALGNAME="SMAUG-T5_KEM")
target_compile_definitions(${KAT_KEM_LIB_NAME5} PUBLIC SMAUG_MODE=5)

add_executable(${LIB_NAME5}-kat PQCgenKAT_kem.c)
target_link_libraries(${LIB_NAME5}-kat ${KAT_KEM_LIB_NAME5} ${LIB_NAME5} OpenSSL::Crypto)
