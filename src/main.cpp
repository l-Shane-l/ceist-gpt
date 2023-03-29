#include "caint-azure.hpp"
#include "ceist-gpt.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>
#include <vector>

// Add necessary headers
#include <stdlib.h>

// Add necessary namespaces

using namespace std;

// Define the API endpoint and subscription key
const std::string endpoint =
    "https://westeurope.tts.speech.microsoft.com/cognitiveservices/v1";

int main() {
  // Load API key from config file.
  nlohmann::json config;
  try {
    std::ifstream config_file("../config.json");
    if (!config_file.is_open()) {
      throw std::runtime_error("Failed to open config file");
    }
    config_file >> config;
  } catch (const std::exception &e) {
    spdlog::error("Failed to load config file: {}", e.what());
    return 1;
  }

  std::string api_key;
  std::string azure_tts_key;
  std::string azure_tts_region;
  try {
    api_key = config.at("api_key").get<std::string>();
    azure_tts_key = config.at("azure_tts_key").get<std::string>();
    azure_tts_region = config.at("azure_tts_region").get<std::string>();
  } catch (const std::exception &e) {
    spdlog::error("Failed to read API key from config file: {}", e.what());
    return 1;
  }

  // Initialize logger.
  auto console = spdlog::stdout_color_mt("console");
  console->set_level(spdlog::level::info);

  // Initialize GPT object.
  ceist_gpt::CeistGPT gpt(api_key);

  std::string conversation_history =
      "You are an Irish speak talking to a friend who has some Irish but it's "
      "broken.You believe in speaking the language as much as possible to "
      "help others learn to speak the language better. So you always answer and ask questions in Irish"
      // "and provide the english translation in brackets."
      "The student will start by saying "
      "hello, repond and try to stick to your frineds level of Irish. ";
  // std::string conversation_history = " You are a software engineer working
  // mainly in c, c++ and bash, you answer questions and generate code where
  // applicable";
  while (true) {
    // Prompt the user for a question.
    console->info("Enter a prompt (or \"quit\" to exit): ");
    std::string prompt;
    std::getline(std::cin, prompt);
    if (prompt == "quit") {
      break;
    }

    // Generate response text from GPT.
    int max_tokens = 400;
    double temperature = 1;
    try {
      std::string response_json =
          gpt.query(prompt, conversation_history, max_tokens, temperature);
      nlohmann::json response = nlohmann::json::parse(response_json);
      std::string response_text =
          response["choices"][0]["text"].get<std::string>();
      response_text.erase(0, response_text.find_first_not_of("\n"));
      response_text.erase(response_text.find_last_not_of("\n") + 1);
      std::string voice = "ga-IE-OrlaNeural";
      std::string output_file = "./response.wav";
      console->info("Synthesizing speech");
      if (ceist_speech::synthesize_text(azure_tts_key, azure_tts_region,
                                        response_text, voice, output_file)) {
        console->info("Audio file saved as {}", output_file);
      } else {
        console->info("Failed to synthesize speech");
      }
      console->info(
          "Done Synthesizing"); // Update the conversation_history with the user
                                // input and the model's response.
      conversation_history +=
          "\nUser: " + prompt + "\nAssistant: " + response_text;
      console->info("Response text: {}", response_text);
      // Call the synthesize_speech function to generate the audio file
    } catch (const std::exception &e) {
      console->error("Failed to generate response: {}", e.what());
    }
  }

  return 0;
}
