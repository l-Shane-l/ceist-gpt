#include <curl/curl.h>
#include <fstream>
#include "caint-azure.hpp"


size_t write_data(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool synthesize_speech(const std::string &api_key, const std::string &region, const std::string &text, const std::string &voice, const std::string &output_file) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if (curl) {
        std::string url = "https://" + region + ".tts.speech.microsoft.com/cognitiveservices/v1";
        std::string ssml = "<speak version='1.0' xmlns='http://www.w3.org/2001/10/synthesis' xml:lang='en-US'><voice name='" + voice + "'>" + text + "</voice></speak>";
        struct curl_slist* headers = NULL;

        headers = curl_slist_append(headers, "Content-Type: application/ssml+xml");
        headers = curl_slist_append(headers, "X-Microsoft-OutputFormat: audio-16khz-128kbitrate-mono-mp3");
        headers = curl_slist_append(headers, ("Authorization: Bearer " + api_key).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ssml.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            return false;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();

    std::ofstream output(output_file, std::ios::binary);
    output.write(readBuffer.data(), readBuffer.size());
    output.close();
    return true;
}

