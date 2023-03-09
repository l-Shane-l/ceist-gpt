#pragma once

#include <string>
#include <vector>
#include <curl/curl.h>

namespace ceist_gbt {

class CeistGPT {
public:
    explicit CeistGPT(const std::string& api_key);
    virtual ~CeistGPT();

    std::string query(const std::string& prompt, const std::vector<std::string>& additional_context = {}, int max_tokens = 16, double temperature = 0.7);

private:
    CURL* curl_handle_;
    std::string api_key_;

    static size_t curl_write_callback(void* contents, size_t size, size_t nmemb, void* userp);
};

} // namespace ceist_gbt
