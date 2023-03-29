

curl -X POST -H "Content-Type: application/json" \
-H "Authorization: Bearer <<API KEY HERE>>" \
-d '{"prompt": "Write a short story", "max_tokens": 50, "temperature": 0.7, "n": 1, "stop": "\n", "stream": false, "logprobs": null, "echo": true, "presence_penalty": 0, "frequency_penalty": 0, "best_of": 1, "model": "davinci-codex"}' \
https://api.openai.com/v1/engines/davinci-codex/completions
