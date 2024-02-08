include(GoogleTest)

add_executable(${LIB_NAME1}-main main.c)
target_link_libraries(${LIB_NAME1}-main ${LIB_NAME1})

add_executable(${LIB_NAME3}-main main.c)
target_link_libraries(${LIB_NAME3}-main ${LIB_NAME3})

add_executable(${LIB_NAME5}-main main.c)
target_link_libraries(${LIB_NAME5}-main ${LIB_NAME5})


if(ENABLE_SYMMETRIC_90S)
  add_executable(${LIB_NAME1}-main-90s main.c)
  target_link_libraries(${LIB_NAME1}-main-90s ${LIB_90S_NAME1})

  add_executable(${LIB_NAME3}-main-90s main.c)
  target_link_libraries(${LIB_NAME3}-main-90s ${LIB_90S_NAME3})

  add_executable(${LIB_NAME5}-main-90s main.c)
  target_link_libraries(${LIB_NAME5}-main-90s ${LIB_90S_NAME5})
endif()


if(NOT CMAKE_BUILD_TYPE MATCHES Release)
  add_executable(${LIB_NAME1}-test UnitTest.cpp)
  target_link_libraries(${LIB_NAME1}-test gtest_main ${LIB_NAME1})
  gtest_discover_tests(${LIB_NAME1}-test XML_OUTPUT_DIR ctest_res)

  add_executable(${LIB_NAME3}-test UnitTest.cpp)
  target_link_libraries(${LIB_NAME3}-test gtest_main ${LIB_NAME3})
  gtest_discover_tests(${LIB_NAME3}-test XML_OUTPUT_DIR ctest_res)

  add_executable(${LIB_NAME5}-test UnitTest.cpp)
  target_link_libraries(${LIB_NAME5}-test gtest_main ${LIB_NAME5})
  gtest_discover_tests(${LIB_NAME5}-test XML_OUTPUT_DIR ctest_res)

  if(ENABLE_SYMMETRIC_90S)
    add_executable(${LIB_NAME1}-test-90s UnitTest.cpp)
    target_link_libraries(${LIB_NAME1}-test-90s gtest_main ${LIB_90S_NAME1})
    gtest_discover_tests(${LIB_NAME1}-test-90s XML_OUTPUT_DIR ctest_res)

    add_executable(${LIB_NAME3}-test-90s UnitTest.cpp)
    target_link_libraries(${LIB_NAME3}-test-90s gtest_main ${LIB_90S_NAME3})
    gtest_discover_tests(${LIB_NAME3}-test-90s XML_OUTPUT_DIR ctest_res)

    add_executable(${LIB_NAME5}-test-90s UnitTest.cpp)
    target_link_libraries(${LIB_NAME5}-test-90s gtest_main ${LIB_90S_NAME5})
    gtest_discover_tests(${LIB_NAME5}-test-90s XML_OUTPUT_DIR ctest_res)
  endif()
endif()