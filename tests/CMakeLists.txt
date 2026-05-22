add_executable(
  sjson_deserializer_test
  sjson_deserializer_test.cpp
)

include_directories(
  ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  sjson_deserializer_test
  SJsonLib
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(sjson_deserializer_test)
