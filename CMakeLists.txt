cmake_minimum_required(VERSION 3.16)
project(ceist-gbt VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

# Dependencies
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

# Library
add_library(ceist-gbt
  src/ceist-gbt.cpp
  include/ceist-gbt.hpp
)

target_include_directories(ceist-gbt PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(ceist-gbt PRIVATE
  ${CONAN_LIBS}
  ${OPENSSL_LIBRARIES}
  ${Boost_LIBRARIES}
  curl
)

# Executable
add_executable(ceist-gbt-cli
  src/main.cpp
)

target_link_libraries(ceist-gbt-cli PRIVATE
  ceist-gbt
)

# Testing
enable_testing()
add_subdirectory(test)
