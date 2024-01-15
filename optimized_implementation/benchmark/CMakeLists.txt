set(BENCH_NAME1 ${PROJECT_NAME}1-benchmark)
add_executable(${BENCH_NAME1} speed.c cpucycles.c speed_print.c)
target_link_libraries(${BENCH_NAME1} ${LIB_NAME1} OpenSSL::Crypto)
target_include_directories(${BENCH_NAME1} PUBLIC ${PROJECT_SOURCE_DIR}/include)

set(BENCH_NAME3 ${PROJECT_NAME}3-benchmark)
add_executable(${BENCH_NAME3} speed.c cpucycles.c speed_print.c)
target_link_libraries(${BENCH_NAME3} ${LIB_NAME3} OpenSSL::Crypto)
target_include_directories(${BENCH_NAME3} PUBLIC ${PROJECT_SOURCE_DIR}/include)

set(BENCH_NAME5 ${PROJECT_NAME}5-benchmark)
add_executable(${BENCH_NAME5} speed.c cpucycles.c speed_print.c)
target_link_libraries(${BENCH_NAME5} ${LIB_NAME5} OpenSSL::Crypto)
target_include_directories(${BENCH_NAME5} PUBLIC ${PROJECT_SOURCE_DIR}/include)