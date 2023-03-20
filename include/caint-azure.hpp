// ceist-speech.hpp

#ifndef CEIST_SPEECH_HPP
#define CEIST_SPEECH_HPP

#include <string>

namespace ceist_speech {

bool synthesize_text(const std::string& api_key, const std::string& region, const std::string& text, const std::string& voice, const std::string& output_file);

} // namespace ceist_speech

#endif // CEIST_SPEECH_HPP

