cmake_minimum_required(VERSION 3.5)

# Set project name
project(ceist-gpt)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")


# Find required libraries
find_package(Boost 1.74.0 REQUIRED COMPONENTS program_options)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
find_package(spdlog REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAO REQUIRED ao)

# Include directories
include_directories(${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${spdlog_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS} ${LIBAO_INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}/include")
# Add source files
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/ceist-gpt.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/caint-azure.cpp" )

# Create library
add_library(ceist-gpt ${SOURCES})

# Create executable
add_executable(ceist-gpt-app "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Link libraries
target_link_libraries(ceist-gpt-app ceist-gpt spdlog::spdlog Boost::program_options ${LIBAO_LIBRARIES} ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES})
