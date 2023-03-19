#pragma once

#include <string>

bool synthesize_speech(const std::string &api_key, const std::string &region, const std::string &text, const std::string &voice, const std::string &output_file);

