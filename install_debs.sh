#!/bin/bash

# Install build tools
sudo apt-get update
sudo apt-get install -y build-essential cmake

# Install Clang
sudo apt-get install -y clang

# Install Clang-tidy
sudo apt-get install -y clang-tidy

# Install Cpplint
sudo apt-get install -y python-pip
sudo pip install cpplint

# Install Cppcheck
sudo apt-get install -y cppcheck

# Install Include-what-you-use
sudo apt-get install -y iwyu

# Install Valgrind
sudo apt-get install -y valgrind

# Install Boost libraries
sudo apt-get install -y libboost-all-dev

# Install OpenSSL
sudo apt-get install -y libssl-dev

# Install RapidJSON
sudo apt-get install -y rapidjson-dev

# Install spdlog
sudo apt-get install -y libspdlog-dev

# Install Catch2
sudo apt-get install -y catch2
