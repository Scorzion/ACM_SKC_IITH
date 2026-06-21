#include <iostream>
#include <string>
#include <cctype>

std::string decrypt(const std::string& text, const std::string& key) {
    std::string result = "";
    int key_len = key.length();
    int key_idx = 0;
    
    for (char c : text) {
        if (std::isalpha(c)) {
            char base = std::isupper(c) ? 'A' : 'a';
            char k_char = key[key_idx % key_len];
            int shift = std::toupper(k_char) - 'A';
            int val = (c - base - shift + 26) % 26;
            result += val + base;
            key_idx++;
        } else {
            result += c;
        }
    }
    return result;
}

int main() {
    std::string ciphertext = "CHREEVOAHMAERATBIAXXWTNXBEEOPHBSBQMQEQERBWRVXUOAKXAOSXXWEAHBWGJMMQMNKGRFVGXWTRZXWIAKLXFPSKAUTEMNDCMGTSXMXBTUIADNGMGPSRELXNJELXVRVPRTULHDNQWTWDTYGBPHXTFALJHASVBFXNGLLCHRZBWELEKMSJIKNBHWRJGNMGJSGLXFEYPHAGNRBIEQJTAMRVLCRREMNDGLXRRIMGNSNRWCHRQHAEYEVTAQEBBIPEEWEVKAKOEWADREMXMTBHHCHRTKDNVRZCHRCLQOHPWQAIIWXNRMGWOIIFKEE";

    std::string key = "JANET";

    std::cout << "--- Vigenere Cipher Decryption (Key = " << key << ") ---\n";
    std::cout << decrypt(ciphertext, key) << "\n";
    return 0;
}
