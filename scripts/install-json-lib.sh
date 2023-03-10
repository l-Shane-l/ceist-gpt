#!/bin/bash

sudo apt-get install libcurl4-openssl-dev

# Clone the nlohmann_json repository
git clone https://github.com/nlohmann/json.git

# Create a build directory
mkdir -p json/build
cd json/build

# Configure the build
cmake ..

# Build and install the library
make install
