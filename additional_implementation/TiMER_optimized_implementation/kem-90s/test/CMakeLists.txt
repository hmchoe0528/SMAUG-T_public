include(GoogleTest)

add_executable(${LIB_NAME1}-main main.c)
target_link_libraries(${LIB_NAME1}-main ${LIB_NAME1})

if(NOT CMAKE_BUILD_TYPE MATCHES Release)
  add_executable(${LIB_NAME1}-test UnitTest.cpp)
  target_compile_definitions(${LIB_NAME1}-test PUBLIC AVX_90S)
  target_link_libraries(${LIB_NAME1}-test gtest_main ${LIB_NAME1})
  gtest_discover_tests(${LIB_NAME1}-test XML_OUTPUT_DIR ctest_res)
endif()

