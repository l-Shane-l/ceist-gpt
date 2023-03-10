#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "ceist-gpt.hpp"

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
        std::cerr << "Failed to load config file: " << e.what() << std::endl;
        return 1;
    }

    std::string api_key;
    try {
        api_key = config.at("api_key").get<std::string>();
    } catch (const std::exception& e) {
        std::cerr << "Failed to read API key from config file: " << e.what() << std::endl;
        return 1;
    }

    // Initialize GPT object.
    ceist_gpt::CeistGPT gpt(api_key);

    while (true) {
        // Prompt the user for a question.
        std::cout << "Enter a prompt (or \"quit\" to exit): ";
        std::string prompt;
        std::getline(std::cin, prompt);
        if (prompt == "quit") {
            break;
        }

        // Generate response text from GPT.
        std::vector<std::string> additional_context = {};
        int max_tokens = 4000;
        double temperature = 1;
        try {
            std::string response_text = gpt.query(prompt, additional_context, max_tokens, temperature);
            std::cout << "Response text: " << response_text << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Failed to generate response: " << e.what() << std::endl;
        }
    }

    return 0;
}
