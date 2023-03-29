#include "caint-azure.hpp"
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <ao/ao.h>
#include <nlohmann/json.hpp>
#include <sstream>
#include <spdlog/spdlog.h>

namespace ceist_speech {

  size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp) {
  std::istream *stream = reinterpret_cast<std::istream *>(userp);
  stream->read(static_cast<char *>(ptr), size * nmemb);
  return static_cast<size_t>(stream->gcount());
}


size_t curl_write_callback(void *contents, size_t size, size_t nmemb,
                           void *userp) {
  std::string *response_data = (std::string *)userp;
  size_t realsize = size * nmemb;
  response_data->append((char *)contents, realsize);

  // std::cout << "Received data: " << std::string((char *)contents, realsize) << std::endl;

  return realsize;
}

void play_audio(std::vector<char>& audio_data) {
    ao_initialize();

    int driver = ao_default_driver_id();
    if (driver == -1) {
        std::cerr << "Failed to get default audio driver" << std::endl;
        ao_shutdown();
        return;
    }

    ao_sample_format format;
    memset(&format, 0, sizeof(format));
    format.bits = 16;
    format.channels = 1;
    format.rate = 24000;
    format.byte_format = AO_FMT_NATIVE;

    ao_device* device = ao_open_live(driver, &format, nullptr);
    if (!device) {
        std::cerr << "Failed to open audio device" << std::endl;
        ao_shutdown();
        return;
    }

    ao_play(device, audio_data.data(), audio_data.size());

    ao_close(device);
    ao_shutdown();
}



std::string get_access_token(const std::string &api_key, const std::string &region) {
    CURL *curl_handle = curl_easy_init();
    if (!curl_handle) {
        throw std::runtime_error("Failed to initialize CURL handle");
    }

    std::string token_url = "https://" + region + ".api.cognitive.microsoft.com/sts/v1.0/issuetoken";
    curl_easy_setopt(curl_handle, CURLOPT_URL, token_url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_POST, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curl_write_callback);

struct curl_slist *headers = nullptr;
headers = curl_slist_append(headers, "Content-Type: application/ssml+xml");
// headers = curl_slist_append(headers, "Transfer-Encoding: chunked");
headers = curl_slist_append(headers, "Expect: 100-continue");
headers = curl_slist_append(headers, "Content-Length: 0");


    headers = curl_slist_append(headers, ("Ocp-Apim-Subscription-Key: " + api_key).c_str());
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
    std::string response_string;
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_string);
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 10L); // Set a timeout of 10 seconds
    // std::cout << "Request URL: " << token_url << std::endl;
// std::cout << "Headers:" << std::endl;
// for (curl_slist* header = headers; header != nullptr; header = header->next) {
//     std::cout << header->data << std::endl;
// }
curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);
    CURLcode res = curl_easy_perform(curl_handle);
    if (res != CURLE_OK) {
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl_handle);
        throw std::runtime_error("Failed to get access token: " + std::string(curl_easy_strerror(res)));
    }

    long response_code;
    curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &response_code);
    if (response_code != 200) {
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl_handle);
        throw std::runtime_error("Failed to get access token: server responded with status code " + std::to_string(response_code));
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl_handle);
    // spdlog::info("Access token: {}", response_string);
    return response_string;
}


bool synthesize_text(const std::string &api_key, const std::string &region,
                     const std::string &text, const std::string &voice,
                     const std::string &output_file) {
  std::string access_token = get_access_token(api_key, region);

std::stringstream ssml_input;
ssml_input << "<speak version='1.0' xml:lang='ga-IE'><voice xml:lang='ga-IE' xml:gender='Female' name='ga-IE-OrlaNeural'>"<< text <<"</voice></speak>";
// spdlog::info("SSML input: {}", ssml_input.str());

std::string ssml_string = ssml_input.str();

std::string endpoint ="https://" + region + ".tts.speech.microsoft.com/cognitiveservices/v1";
std::string output_format = "riff-24khz-16bit-mono-pcm";

CURL *curl_handle = curl_easy_init();
curl_easy_setopt(curl_handle, CURLOPT_URL, endpoint.c_str());
curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)ssml_string.length());

curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, ssml_string.c_str());
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curl_write_callback);
  curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(curl_handle, CURLOPT_POST, 1L);
  struct curl_slist *headers = nullptr;
  headers = curl_slist_append(headers, "Content-Type: application/ssml+xml");
  // headers = curl_slist_append(headers, "Transfer-Encoding: chunked");
// headers = curl_slist_append(headers, "Expect: 100-continue");
  headers = curl_slist_append(headers, ("Authorization: Bearer "+ access_token ).c_str());
  headers = curl_slist_append(headers, ("X-Microsoft-OutputFormat: "+ output_format).c_str());
  
  headers = curl_slist_append(headers, ("User-Agent: test"));

  curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);
  

  std::string response_data;
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_data);
  CURLcode res = curl_easy_perform(curl_handle);
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl_handle);

  if (res != CURLE_OK) {
    std::cerr << "Failed to synthesize speech" << std::endl;
    return false;
  }

  std::ofstream output_stream(output_file, std::ios::binary);
  if (!output_stream) {
    std::cerr << "Unable to open output file" << std::endl;
    return false;
  }

  // Convert the response_data (std::string) to a std::vector<char>
std::vector<char> audio_data(response_data.begin(), response_data.end());

// Play the audio
play_audio(audio_data);

  output_stream.write(response_data.data(), response_data.size());
  output_stream.close();
  

  std::cout << "Speech synthesized successfully. Output saved to "
            << output_file << std::endl;
  return true;
}


} // namespace ceist_speech
