include(GoogleTest)

add_executable(${LIB_NAME1}-main main.c)
target_link_libraries(${LIB_NAME1}-main ${LIB_NAME1})

add_executable(${LIB_NAME3}-main main.c)
target_link_libraries(${LIB_NAME3}-main ${LIB_NAME3})

add_executable(${LIB_NAME5}-main main.c)
target_link_libraries(${LIB_NAME5}-main ${LIB_NAME5})


if(NOT CMAKE_BUILD_TYPE MATCHES Release)
  add_executable(${LIB_NAME1}-test UnitTest.cpp)
  target_compile_definitions(${LIB_NAME1}-test PUBLIC AVX_90S)
  target_link_libraries(${LIB_NAME1}-test gtest_main ${LIB_NAME1})
  gtest_discover_tests(${LIB_NAME1}-test XML_OUTPUT_DIR ctest_res)

  add_executable(${LIB_NAME3}-test UnitTest.cpp)
  target_compile_definitions(${LIB_NAME3}-test PUBLIC AVX_90S)
  target_link_libraries(${LIB_NAME3}-test gtest_main ${LIB_NAME3})
  gtest_discover_tests(${LIB_NAME3}-test XML_OUTPUT_DIR ctest_res)

  add_executable(${LIB_NAME5}-test UnitTest.cpp)
  target_compile_definitions(${LIB_NAME5}-test PUBLIC AVX_90S)
  target_link_libraries(${LIB_NAME5}-test gtest_main ${LIB_NAME5})
  gtest_discover_tests(${LIB_NAME5}-test XML_OUTPUT_DIR ctest_res)
endif()

