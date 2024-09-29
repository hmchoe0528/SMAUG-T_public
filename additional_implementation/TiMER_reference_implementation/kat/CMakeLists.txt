set(kat_srcs
    ${PROJECT_SOURCE_DIR}/kat/kat_kem.c
    ${PROJECT_SOURCE_DIR}/src/rng.c
)

find_package(OpenSSL REQUIRED)


set(KAT_KEM_LIB_NAME1 ${PROJECT_NAME}1-kat-kem)
add_library(${KAT_KEM_LIB_NAME1} SHARED ${kat_srcs})
target_include_directories(${KAT_KEM_LIB_NAME1} PUBLIC ../include)
target_compile_definitions(${KAT_KEM_LIB_NAME1} PUBLIC CRYPTO_ALGNAME="TiMER_KEM")
target_compile_definitions(${KAT_KEM_LIB_NAME1} PUBLIC TiMER_MODE=1)

add_executable(${LIB_NAME1}-kat PQCgenKAT_kem.c)
target_link_libraries(${LIB_NAME1}-kat ${KAT_KEM_LIB_NAME1} ${LIB_NAME1} OpenSSL::Crypto)
