#include "ceist-gpt.hpp"
#include <iostream>

namespace ceist_gpt {

CeistGPT::CeistGPT(const std::string& api_key) : api_key_(api_key), curl_handle_(nullptr) {
    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle_ = curl_easy_init();
}

CeistGPT::~CeistGPT() {
    curl_easy_cleanup(curl_handle_);
    curl_global_cleanup();
}

std::string CeistGPT::query(const std::string& prompt, const std::vector<std::string>& additional_context, int max_tokens, double temperature) {
    // Construct the OpenAI API request payload as a JSON object
    nlohmann::json request;
    request["prompt"] = prompt;
    request["max_tokens"] = max_tokens;
    request["temperature"] = temperature;
    for_each(additional_context.begin(), additional_context.end(), [](std::string x){std::cout<<x<<std::endl;});
    if (!additional_context.empty()) {
        request["context"] = additional_context;
    }

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

