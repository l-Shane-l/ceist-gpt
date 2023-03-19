#ifndef AUTH_H
#define AUTH_H

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class OpenAIAuthentication
{
public:
    OpenAIAuthentication() : apiKey("") {}
    OpenAIAuthentication(const std::string& configPath) {
        // Load apiKey from config file
        std::ifstream configFile(configPath);
        if (configFile.is_open()) {
            json configJson;
            configFile >> configJson;
            if (configJson.contains("api_key")) {
                apiKey = configJson["api_key"];
            }
            configFile.close();
        }
    }
    void setApiKey(const std::string& apiKey) { this->apiKey = apiKey; }
    const std::string& getApiKey() const { return apiKey; }
private:
    std::string apiKey;
};

#endif

