cmake_minimum_required(VERSION 3.5)

# Set project name
project(ceist-gpt)

# Find required libraries
find_package(Boost 1.74.0 REQUIRED COMPONENTS program_options)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)

# Include directories
include_directories(${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${CURL_INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Add source files
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create library
add_library(ceist-gpt ${SOURCES})

# Create executable
add_executable(ceist-gpt-app "${CMAKE_CURRENT_SOURCE_DIR}/src/ceist-gpt.cpp")

# Link libraries
target_link_libraries(ceist-gpt-app ceist-gpt Boost::program_options ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES})
