cmake_minimum_required(VERSION 3.16)

find_package(Catch2 REQUIRED)

add_executable(ceist-gpt-test
    ceist-gpt-test.cpp
)

target_link_libraries(ceist-gpt-test PRIVATE
    ceist-gpt
    ${CONAN_LIBS}
    ${OPENSSL_LIBRARIES}
    ${Boost_LIBRARIES}
    curl
    Catch2::Catch2
)

include(CTest)
include(Catch)
catch_discover_tests(ceist-gpt-test)

