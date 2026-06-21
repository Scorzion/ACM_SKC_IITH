#include <iostream>
#include <string>

std::string decrypt(const std::string& text, int key) {
    std::string result = "";
    key = (key % 26 + 26) % 26;
    for (char c : text) {
        if (std::isalpha(c)) {
            char base = std::isupper(c) ? 'A' : 'a';
            result += (c - base - key + 26) % 26 + base;
        } else {
            result += c;
        }
    }
    return result;
}

int main() {
    std::string ciphertext = "Ynulpkcnwlddu eo w bqjzwiajpwh pkkh ej ejbkniwpekj oayqnepu. Ep dahlo lnkpayp iaoowcao bnki qjwqpdknevaz wyyaoo xu pnwjobkniejc lhwej pazp ejpk wj qjnawzwxha bkni. Whpdkqcd ikzanj yeldano wna iqyd ikna ykilhaz, deopkneywh oydaiao oqyd wo pda odebp yeldan naiwej qoabqh bkn hawnjejc pda xwoeyo kb ajynulpekj wjz ynulpwjwhuoeo.";

    std::cout << "--- Shift Cipher Decryption (Key = 22) ---\n";
    std::cout << decrypt(ciphertext, 22) << "\n";
    return 0;
}
