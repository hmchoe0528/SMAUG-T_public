include(GoogleTest)

find_package(OpenSSL REQUIRED)

add_executable(${LIB_NAME1}-main main.c)
target_link_libraries(${LIB_NAME1}-main ${LIB_NAME1} OpenSSL::Crypto)

add_executable(${LIB_NAME3}-main main.c)
target_link_libraries(${LIB_NAME3}-main ${LIB_NAME3} OpenSSL::Crypto)

add_executable(${LIB_NAME5}-main main.c)
target_link_libraries(${LIB_NAME5}-main ${LIB_NAME5} OpenSSL::Crypto)