set(BENCH_NAME1 ${PROJECT_NAME}1-benchmark)
add_executable(${BENCH_NAME1} speed.c cpucycles.c speed_print.c)
target_link_libraries(${BENCH_NAME1} ${LIB_NAME1} OpenSSL::Crypto)
target_include_directories(${BENCH_NAME1} PUBLIC ${PROJECT_SOURCE_DIR}/include)
