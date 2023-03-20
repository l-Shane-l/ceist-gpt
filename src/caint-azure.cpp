// ceist-speech.cpp

#include "caint-azure.hpp"
#include <iostream>
#include <fstream>
#include <speechapi_cxx.h>
#include <speechapi_c.h>
#include <speechapi_cxx.h>
#include <speechapi_cxx_speech_synthesizer.h>
#include <speechapi_cxx_speech_config.h>
#include <speechapi_cxx_common.h>
#include <speechapi_cxx_speech_synthesis_result.h>
#include <speechapi_cxx_speech_synthesis_eventargs.h>

namespace ceist_speech {

bool synthesize_text(const std::string& api_key, const std::string& region, const std::string& text, const std::string& voice, const std::string& output_file) {
    using namespace Microsoft::CognitiveServices::Speech;

    // Create a speech configuration object with the provided API key and region
    auto config = SpeechConfig::FromSubscription(api_key, region);

    // Create a speech synthesizer object with the given configuration
    auto synthesizer = SpeechSynthesizer::FromConfig(config);

    // Set the synthesis options, including the voice
    auto options = SpeechSynthesisOptions::FromLanguage(voice);
    synthesizer->SetSpeechSynthesisVoiceName(voice);

    // Synthesize the given text and write the audio output to the given file
    auto result = synthesizer->SpeakText(text);
    if (result->Reason == ResultReason::SynthesizingAudioCompleted) {
        auto audio_data = result->AudioData();
        output_file.open(output_file, std::ios::binary);
        output_file.write(reinterpret_cast<const char*>(audio_data.data()), audio_data.size());
        output_file.close();
        return true;
    } else {
        std::cerr << "Speech synthesis failed: " << static_cast<int>(result->Reason) << std::endl;
        return false;
    }
}

} // namespace ceist_speech

