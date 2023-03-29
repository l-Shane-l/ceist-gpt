// ceist-speech.hpp
#ifndef CEIST_AZURE_HPP
#define CEIST_AZURE_HPP

#include <string>

namespace ceist_speech {

// Function to get the access token from the Microsoft API
std::string get_access_token(const std::string& api_key, const std::string& region);

// Function to synthesize speech from text using the Microsoft Speech Service
bool synthesize_text(const std::string& api_key, const std::string& region, const std::string& text, const std::string& voice, const std::string& output_file);

} // namespace ceist_speech

#endif // CEIST_AZURE_HPP

