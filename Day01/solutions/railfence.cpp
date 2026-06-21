#include <iostream>
#include <string>
#include <vector>

std::string encrypt(const std::string& text, int key) {
    if (key <= 1) return text;
    std::vector<std::string> rails(key);
    int rail = 0;
    bool down = true;
    
    for (char c : text) {
        rails[rail] += c;
        if (rail == 0) down = true;
        else if (rail == key - 1) down = false;
        rail += down ? 1 : -1;
    }
    
    std::string result = "";
    for (const auto& r : rails) {
        result += r;
    }
    return result;
}

std::string decrypt(const std::string& text, int key) {
    if (key <= 1) return text;
    int L = text.length();
    std::vector<int> sizes(key, 0);
    int rail = 0;
    bool down = true;
    
    for (int i = 0; i < L; ++i) {
        sizes[rail]++;
        if (rail == 0) down = true;
        else if (rail == key - 1) down = false;
        rail += down ? 1 : -1;
    }
    
    std::vector<std::string> rails(key);
    int idx = 0;
    for (int r = 0; r < key; ++r) {
        rails[r] = text.substr(idx, sizes[r]);
        idx += sizes[r];
    }
    
    std::string result = "";
    std::vector<int> ptrs(key, 0);
    rail = 0;
    down = true;
    for (int i = 0; i < L; ++i) {
        result += rails[rail][ptrs[rail]++];
        if (rail == 0) down = true;
        else if (rail == key - 1) down = false;
        rail += down ? 1 : -1;
    }
    return result;
}

int main() {
    std::string plaintext = "HELLOWORLD";
    std::cout << "--- Rail Fence Cipher (Plaintext: " << plaintext << ") ---\n";

    for (int key : {3, 4}) {
        std::string ct = encrypt(plaintext, key);
        std::string pt = decrypt(ct, key);
        std::cout << "Key (Rails): " << key << "\n";
        std::cout << "  Ciphertext: " << ct << "\n";
        std::cout << "  Decrypted:  " << pt << "\n\n";
    }
    return 0;
}
