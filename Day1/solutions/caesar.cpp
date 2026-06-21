#include <iostream>
#include <string>

std::string decrypt(const std::string& text) {
    std::string result = "";
    for (char c : text) {
        if (std::isalpha(c)) {
            char base = std::isupper(c) ? 'A' : 'a';
            result += (c - base - 3 + 26) % 26 + base;
        } else {
            result += c;
        }
    }
    return result;
}

int main() {
    std::string ciphertext = 
        "Fdhvdu flskhu lv rqh ri wkh hduolhvw dqg vlpsohvw phwkrgv ri\n"
        "hqfubswlrq. Lw zrunv eb vkliwlqj hdfk ohwwhu lq wkh sodlqwhaw eB d\n"
        "ilahg qxpehu ri srvlwlrqv lq wkh doskdehw. Dowkrxjk lw lv hdvb wr\n"
        "lpsohphqw, lw surylghv yhub olwwoh vhfxulwb djdlqvw prghuq\n"
        "fubswdqdobvlv whfkqltxhv. Qhyhuwkhohvv, lw uhpdlqv d xvhixo\n"
        "hgxfdwlrqdo wrro iru xqghuvwdqglqj wkh edvlf frqfhswv ri fodvvlfdo\n"
        "fubswrjudskb.";

    std::cout << "--- Caesar Cipher Decryption ---\n";
    std::cout << decrypt(ciphertext) << "\n";
    return 0;
}
