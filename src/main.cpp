#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include "ceist-gpt.hpp"
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main() {
    // Load API key from config file.
    nlohmann::json config;
    try {
        std::ifstream config_file("../config.json");
        if (!config_file.is_open()) {
            throw std::runtime_error("Failed to open config file");
        }
        config_file >> config;
    } catch (const std::exception& e) {
        spdlog::error("Failed to load config file: {}", e.what());
        return 1;
    }

    std::string api_key;
    try {
        api_key = config.at("api_key").get<std::string>();
    } catch (const std::exception& e) {
        spdlog::error("Failed to read API key from config file: {}", e.what());
        return 1;
    }

    // Initialize logger.
    auto console = spdlog::stdout_color_mt("console");
    console->set_level(spdlog::level::info);

    // Initialize GPT object.
    ceist_gpt::CeistGPT gpt(api_key);

    std::string conversation_history;
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
            std::string response_json = gpt.query(prompt, conversation_history, max_tokens, temperature);
            nlohmann::json response = nlohmann::json::parse(response_json);
            std::string response_text = response["choices"][0]["text"].get<std::string>();
            response_text.erase(0, response_text.find_first_not_of("\n"));
            response_text.erase(response_text.find_last_not_of("\n") + 1);

            // Update the conversation_history with the user input and the model's response.
            conversation_history += "\nUser: " + prompt + "\nAssistant: " + response_text;

            console->info("Response text: {}", response_text);
        } catch (const std::exception& e) {
            console->error("Failed to generate response: {}", e.what());
        }
    }

    return 0;
}

