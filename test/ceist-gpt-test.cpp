#include <ceist-gpt.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Test CeistGpt API query", "[ceist-gpt]") {
    ceist::CeistGpt ceist_gpt;
    std::string prompt = "Hello,";
    std::string response = ceist_gpt.query(prompt);
    REQUIRE(!response.empty());
}

// Add more test cases here...

