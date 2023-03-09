# Add GoogleTest.
add_subdirectory(googletest)

# Add test executable.
add_executable(tests
    ceist-gpt-test.cpp
)

# Link with CeistGpt library and GoogleTest.
target_link_libraries(tests PRIVATE
    ceist-gpt
    gtest
    gtest_main
)

# Add tests to CTest.
add_test(
    NAME CeistGptTest
    COMMAND tests
)
