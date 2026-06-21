// Template C++ client for the 4-round DES oracle.
//
// It sends a chosen plaintext to the oracle's JSON API and prints the
// ciphertext. Use it as the starting point for your differential attack: query
// pairs of plaintexts, collect the ciphertexts, and feed them into your
// analysis.
//
// API contract:
//   POST http://<SERVER_IP>/api/encrypt
//   request  body: {"plaintext": "0123456789ABCDEF"}   (16 hex chars, 64 bits)
//   response body: {"ciphertext": "985ED89996608AEF"}
//   HTTP 429 is returned once you exceed the per-IP daily query limit.
//
// Build (Linux/macOS):
//   sudo apt install libcurl4-openssl-dev      # Debian/Ubuntu
//   # brew install curl                        # macOS (then add -I/-L flags)
//   g++ -std=c++17 query.cpp -o query -lcurl
//
// Run:
//   ./query                       # runs the demo in main()
//
// Only the C++ standard library and libcurl are required.

#include <curl/curl.h>

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

// ---- CONFIGURATION -------------------------------------------------------
// Replace with the oracle address given by your instructor, e.g.
//   "http://192.168.1.50"  or  "http://10.0.0.5:80"
static const std::string BASE_URL = "http://<SERVER_IP>";

// ---- HTTP plumbing -------------------------------------------------------

// libcurl write callback: append received bytes to a std::string.
static size_t write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    auto *out = static_cast<std::string *>(userdata);
    out->append(ptr, size * nmemb);
    return size * nmemb;
}

struct HttpResponse {
    long status = 0;       // HTTP status code (e.g. 200, 429)
    std::string body;      // raw response body
};

// POST a JSON string to `url`. Throws std::runtime_error on transport failure.
static HttpResponse http_post_json(const std::string &url, const std::string &json) {
    CURL *curl = curl_easy_init();
    if (!curl) throw std::runtime_error("curl_easy_init failed");

    HttpResponse resp;
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(json.size()));
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp.body);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);

    CURLcode rc = curl_easy_perform(curl);
    if (rc == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resp.status);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (rc != CURLE_OK) {
        throw std::runtime_error(std::string("HTTP request failed: ") +
                                 curl_easy_strerror(rc));
    }
    return resp;
}

// ---- tiny JSON field extractor ------------------------------------------
// The API always returns flat objects like {"ciphertext": "..."} or
// {"detail": "..."}, so a minimal extractor is enough (no JSON library).
static std::string extract_json_string(const std::string &body, const std::string &key) {
    const std::string needle = "\"" + key + "\"";
    size_t k = body.find(needle);
    if (k == std::string::npos) return "";
    size_t colon = body.find(':', k + needle.size());
    if (colon == std::string::npos) return "";
    size_t q1 = body.find('"', colon + 1);
    if (q1 == std::string::npos) return "";
    size_t q2 = body.find('"', q1 + 1);
    if (q2 == std::string::npos) return "";
    return body.substr(q1 + 1, q2 - q1 - 1);
}

// ---- oracle query --------------------------------------------------------

// Encrypt a 16-hex-character plaintext. Returns the uppercase ciphertext hex.
// Throws std::runtime_error on rate-limit (429) or any oracle/transport error.
std::string query_oracle(const std::string &plaintext_hex) {
    if (plaintext_hex.size() != 16) {
        throw std::runtime_error("plaintext must be exactly 16 hex characters");
    }
    const std::string url = BASE_URL + "/api/encrypt";
    const std::string payload = "{\"plaintext\": \"" + plaintext_hex + "\"}";

    HttpResponse resp = http_post_json(url, payload);

    if (resp.status == 429) {
        throw std::runtime_error("rate limit reached (per-IP daily quota exceeded)");
    }
    if (resp.status != 200) {
        std::string detail = extract_json_string(resp.body, "detail");
        throw std::runtime_error("oracle error (HTTP " + std::to_string(resp.status) +
                                 "): " + (detail.empty() ? resp.body : detail));
    }

    std::string ct = extract_json_string(resp.body, "ciphertext");
    if (ct.size() != 16) {
        throw std::runtime_error("unexpected response body: " + resp.body);
    }
    return ct;
}

// Convenience: XOR two 64-bit values given as 16-hex-char strings, returns hex.
// Handy for forming a plaintext pair P* = P XOR delta in a differential attack.
std::string xor_hex64(const std::string &a_hex, const std::string &b_hex) {
    uint64_t a = std::stoull(a_hex, nullptr, 16);
    uint64_t b = std::stoull(b_hex, nullptr, 16);
    char buf[17];
    std::snprintf(buf, sizeof(buf), "%016llX",
                  static_cast<unsigned long long>(a ^ b));
    return std::string(buf);
}

// ---- demo ----------------------------------------------------------------

int main() {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    try {
        // 1) Single query.
        const std::string pt = "0123456789ABCDEF";
        std::string ct = query_oracle(pt);
        std::cout << "PT=0x" << pt << "  ->  CT=0x" << ct << "\n";

        // 2) A chosen-plaintext PAIR with a fixed input difference (delta).
        //    Replace `delta` with the input XOR your characteristic requires.
        const std::string delta = "0400000000000000";   // example: L0' only
        std::string pt2 = xor_hex64(pt, delta);
        std::string ct2 = query_oracle(pt2);
        std::cout << "PT*=0x" << pt2 << "  ->  CT*=0x" << ct2 << "\n";
        std::cout << "CT XOR CT* = 0x" << xor_hex64(ct, ct2) << "\n";

        // TODO (students): loop over many pairs here, store (CT, CT*) and run
        // your last-round S-box key-recovery counting.
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        curl_global_cleanup();
        return 1;
    }

    curl_global_cleanup();
    return 0;
}
