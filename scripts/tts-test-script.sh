#!/bin/bash

# Define the API endpoint and subscription key
endpoint="https://westeurope.tts.speech.microsoft.com/cognitiveservices/v1"
subscription_key="YOUR_SUBSCRIPTION_KEY"

# Define the input SSML text and output audio format
input_ssml="<speak version='1.0' xml:lang='ga-IE'><voice xml:lang='ga-IE' xml:gender='Male'
    name='ga-IE-OrlaNeural'>
        Conas atá tú?, Tá mé go maith, agus tú? Is maith liom an ceol a chloisim. Is mise Orla.
</voice></speak>"


output_format="riff-24khz-16bit-mono-pcm"

# Get the access token using the subscription key

access_token=$(curl -s -X POST "https://westeurope.api.cognitive.microsoft.com/sts/v1.0/issuetoken" -H "Content-type: application/ssml+xml" -H "Content-Length: 0" -H "Ocp-Apim-Subscription-Key: $subscription_key")

# Send a request to the API to synthesize speech from the input text
curl -v -X POST \
    --header "Content-Type: application/ssml+xml" \
    --header "Authorization: Bearer $access_token" \
    --header "X-Microsoft-OutputFormat: $output_format" \
    --data "$input_ssml" \
    -o output.wav \
    -D response_headers.txt \
    "$endpoint"

# Check if the API request was successful
if [ $? -eq 0 ]; then
    echo "Speech synthesized successfully. Output saved to output.mp3"
else
    echo "Failed to synthesize speech"
fi

