#pragma once

#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

namespace ceist_gpt {

class CeistGPT {
public:
    CeistGPT(const std::string& api_key);
    ~CeistGPT();

    std::string query(const std::string& prompt, const std::string& conversation_history, int max_tokens = 150, double temperature = 0.8);

private:
    std::string api_key_;
    CURL* curl_handle_;

    static size_t curl_write_callback(void* contents, size_t size, size_t nmemb, void* userp);

    // Helper function to truncate the conversation history to a maximum number of tokens
    static std::string truncate_tokens(const std::string& text, int max_tokens);
};

} // namespace ceist_gpt

