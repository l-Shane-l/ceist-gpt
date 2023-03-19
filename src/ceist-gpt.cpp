#include "ceist-gpt.hpp"
#include <iostream>

namespace ceist_gpt {

// Helper function to truncate the conversation history to a maximum number of tokens
std::string CeistGPT::truncate_tokens(const std::string& text, int max_tokens) {
    // Tokenize the text into separate words.
    std::vector<std::string> tokens;
    std::istringstream iss(text);
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter(tokens));

    // Truncate the tokens and join them back together.
    if (static_cast<int>(tokens.size()) > max_tokens) {
        tokens.erase(tokens.begin(), tokens.end() - max_tokens);
    }
    std::string truncated_text;
    for (const auto& token : tokens) {
        if (!truncated_text.empty()) {
            truncated_text += " ";
        }
        truncated_text += token;
    }
    return truncated_text;
}

CeistGPT::CeistGPT(const std::string& api_key) : api_key_(api_key), curl_handle_(nullptr) {
    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle_ = curl_easy_init();
}

CeistGPT::~CeistGPT() {
    curl_easy_cleanup(curl_handle_);
    curl_global_cleanup();
}

std::string CeistGPT::query(const std::string& prompt, const std::string& conversation_history, int max_tokens, double temperature) {
    // Construct the OpenAI API request payload as a JSON object
    nlohmann::json request;

    // Truncate the conversation_history to a maximum number of tokens
    int max_history_tokens = 4096 - max_tokens - 50; // 50 tokens buffer
    std::string truncated_history = truncate_tokens(conversation_history, max_history_tokens);

    // Format input using the conversation format
    std::string input = truncated_history + "\nUser: " + prompt + "\nAssistant:";
    request["prompt"] = input;
    request["max_tokens"] = max_tokens;
    request["temperature"] = temperature;

    // Serialize the request payload to a string
    std::string request_payload = request.dump();

    // Set up the CURL handle for making the API request
    curl_easy_setopt(curl_handle_, CURLOPT_URL, "https://api.openai.com/v1/engines/text-davinci-003/completions");
    curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDS, request_payload.c_str());
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, curl_write_callback);

    // Set up headers
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("Authorization: Bearer " + api_key_).c_str());
    curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, headers);

    // Make the API request and receive the response
    std::string response_string;
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &response_string);
    CURLcode res = curl_easy_perform(curl_handle_);
    curl_slist_free_all(headers);

    // Check for errors and return the response
    if (res != CURLE_OK) {
        throw std::runtime_error(curl_easy_strerror(res));
    }

    return response_string;
}

size_t CeistGPT::curl_write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    std::string* response_string = reinterpret_cast<std::string*>(userp);
    response_string->append(reinterpret_cast<char*>(contents), realsize);
    return realsize;
}

} // namespace ceist_gpt

