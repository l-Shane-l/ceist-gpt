#include <gtest/gtest.h>
#include <ceist-gpt/ceist-gpt.hpp>

TEST(CeistGptTest, ExampleTest) {
    // Create a CeistGpt instance and call the generate_text method.
    ceist_gpt::CeistGpt ceist_gpt;
    std::string response = ceist_gpt.generate_text("Hello, world!");

    // Check that the response is not empty.
    ASSERT_FALSE(response.empty());
}
