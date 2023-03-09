# Ceist-gbt
Ceist-gbt is a C++ library and command-line tool for interfacing with the OpenAI GPT API. It provides a simple and easy-to-use interface for sending queries to the API and retrieving responses.

## Dependencies
OpenSSL
Boost
curl

## Building
To build Ceist-gbt, you will need to install the necessary dependencies using Conan:

``` 
$ conan install .
```

Then, you can build the library and command-line tool using CMake:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```
## Usage
To use the Ceist-gbt library in your own project, you can link against the ceist-gbt target in your CMakeLists.txt file:

```
add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE ceist-gbt)
```
To use the Ceist-gbt command-line tool, you can run the ceist-gbt-cli executable and provide your API credentials as command-line arguments:

```
$ ./ceist-gbt-cli --api-key=YOUR_API_KEY --prompt="Hello, World!"
```
## TODO
 - [ ] Add support for Clang-tidy
 - [ ] Add support for Valgrind
 - [ ] Add support for Cpplint
 - [ ] Add support for Cppcheck
 - [ ] Add support for Include-what-you-use
 - [ ] Add support for code coverage
 - [ ] Add more APIs to the library
 - [ ] Implement more functions in CeistGpt to support additional API endpoints.
 - [ ] Improve error handling and error reporting in the CeistGpt library.
 - [ ] Implement more test cases for the CeistGpt library, and add code coverage reporting to the build system.
 - [ ] Add support for building the ceist-gpt-cli binary on Windows platforms.
 - [ ] Add support for building the library and binary with different optimization levels (e.g., -O2, -O3, etc.) using CMake.
 - [ ] Add support for building the library and binary with different compiler flags (e.g., -Wall, -Wextra, etc.) using CMake.
 - [ ] Implement logging using a logging library like spdlog or Boost.Log.
 - [ ] Add support for configuring the API endpoint URL using a command-line option or environment variable.
 - [ ] Implement caching of API responses to improve performance.
 - [ ] Implement retries for failed API requests to improve reliability.

