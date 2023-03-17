#ifndef CEIST_GPT_HPP
#define CEIST_GPT_HPP

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>

namespace ceist_gpt {

class CeistGPT {
   public:
    explicit CeistGPT(const std::string& api_key);
    ~CeistGPT();
    std::string query(const std::string& prompt, const std::vector<std::string>& additional_context, int max_tokens, double temperature);

   private:
    static size_t curl_write_callback(void* contents, size_t size, size_t nmemb, void* userp);

    CURL* curl_handle_;
    std::string api_key_;
};

}

#endif  // CEIST_GPT_HPP
