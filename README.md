# ceist-gpt
## This project is underdevelopment and not released or supported yet
 ceist-gpt is a C++ command-line interface for interacting with OpenAI's ChatGPT API and Azure Text-to-Speech API. The project allows you to send queries to ChatGPT and receive text-based responses. Additionally, it uses the Azure Text-to-Speech API to synthesize the generated text as speech and save it to an audio file.

 I intend to split this project into two once finished, one project will be an opensource c++ library for the openai and any relevant azure apis,
 the other project will be an open source chat app that a user can interface with via speech or text in gaeilge. This second project is trying to address problems 
 very common in minority languages, there is little chance to speak the language day to day and its difficult to find resources in the language so the chat app is designed to address that. Ultimately Ill release it in a android and ios app built with flutter using the c++ library.

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
hed to 
## Dependencies
 - libcurl

## Setup
1. Install libcurl, if not already installed. On Ubuntu, you can use the following command:
```
sudo apt-get install libcurl4-openssl-dev
```
2. Clone the repository:
``
git clone https://github.com/l-Shane-l/ceist-gpt.git
``

3. Update the config.json file with your OpenAI API key and Azure Text-to-Speech API key and region:
```
{
    "openai_key": "your_openai_key_here",
    "azure_tts_key": "your_azure_tts_key_here",
    "azure_tts_region": "your_azure_tts_region_here"
}
```
4. Build the project:

```
mkdir build
cd build
conan install ..
cmake ..
make
```
## Usage
Run the ceist-gpt executable from the command line:

```
./ceist-gpt
```
The program will prompt you for input. Type your query and press Enter. The ChatGPT API will generate a response, which will be displayed as text. The Azure Text-to-Speech API will synthesize the response as speech and save it to an audio file, response.mp3.

## License
This project is licensed under the MIT License - see the LICENSE file for details.


