#include "ceist-gpt.hpp"

#include <cstring>
#include <iostream>
#include <sstream>

namespace ceist_gpt {

// Static callback function used by curl to receive the response from the API.
size_t CeistGPT::curl_write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    std::string* response_buffer = static_cast<std::string*>(userp);
    response_buffer->append(static_cast<char*>(contents), realsize);
    return realsize;
}

CeistGPT::CeistGPT(const std::string& api_key) : curl_handle_(curl_easy_init()), api_key_(api_key) {
    if (!curl_handle_) {
        throw std::runtime_error("Failed to initialize curl handle");
    }
}

CeistGPT::~CeistGPT() {
    curl_easy_cleanup(curl_handle_);
}

std::string CeistGPT::query(const std::string& prompt, const std::vector<std::string>& additional_context, int max_tokens, double temperature) {
    // Construct the API endpoint URL.
    std::stringstream url_stream;
    url_stream << "https://api.openai.com/v1/" << "engines/davinci-codex/completions";
    std::string url = url_stream.str();

    // Construct the request headers.
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + api_key_).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Construct the request body.
    std::stringstream request_body_stream;
    request_body_stream << "{\n"
                        << "  \"prompt\": \"" << prompt << "\",\n"
                        << "  \"max_tokens\": " << max_tokens << ",\n"
                        << "  \"temperature\": " << temperature << ",\n"
                        << "  \"n\": 1,\n"
                        << "  \"stop\": \"\\n\",\n"
                        << "  \"stream\": false,\n"
                        << "  \"logprobs\": null,\n"
                        << "  \"echo\": true,\n"
                        << "  \"presence_penalty\": 0,\n"
                        << "  \"frequency_penalty\": 0,\n"
                        << "  \"best_of\": 1,\n"
                        << "  \"model\": \"davinci-codex\"\n";
    if (!additional_context.empty()) {
        request_body_stream << "  \"prompt\": [";
        for (size_t i = 0; i < additional_context.size(); ++i) {
            request_body_stream << "\"" << additional_context[i] << "\"";
            if (i < additional_context.size() - 1) {
                request_body_stream << ",";
            }
        }
        request_body_stream << "]\n";
    }
    request_body_stream << "}\n";
    std::string request_body = request_body_stream.str();

    // Set up the curl request.
    curl_easy_setopt(curl_handle_, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDS, request_body.c_str());
    curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDSIZE, request_body.length());
    curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, curl_write_callback);

    // Send the request and receive the response.
    std::string response_buffer;
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &response_buffer);
    CURLcode res = curl_easy_perform(curl_handle_);
    curl_slist_free_all(headers);

    // Check for errors.
    if (res != CURLE_OK) {
        throw std::runtime_error(curl_easy_strerror(res));
    }

    // Parse the response JSON.
    nlohmann::json response;
    try {
        response = nlohmann::json::parse(response_buffer);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to parse API response as JSON: " + std::string(e.what()));
    }

    // Extract and return the response text.
    try {
        std::string text = response["choices"][0]["text"];
        return text;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to extract response text from API response: " + std::string(e.what()));
    }
}

}  // namespace ceist_gpt

