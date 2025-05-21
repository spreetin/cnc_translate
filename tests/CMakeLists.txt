set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.tar.gz
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    cnc_testing
    parser_test.cpp
    lexer_test.cpp
    test_helpers.h
    parse_node_gen_test.cpp
    process_test.cpp
)
target_link_libraries(
    cnc_testing
    cnc_parse_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cnc_testing)

#add_custom_command(
#  TARGET cnc_testing
#  POST_BUILD
#  COMMAND $<TARGET_FILE:cnc_testing>
#)
