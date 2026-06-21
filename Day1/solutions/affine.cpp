#include <iostream>
#include <string>

int modInverse(int a, int m) {
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1;
}

std::string decrypt(const std::string& text, int a, int b) {
    std::string result = "";
    int a_inv = modInverse(a, 26);
    if (a_inv == -1) return "Error: Multiplicative inverse does not exist.";
    
    for (char c : text) {
        if (std::isalpha(c)) {
            char base = std::isupper(c) ? 'A' : 'a';
            int val = (a_inv * (c - base - b) % 26 + 26) % 26;
            result += val + base;
        } else {
            result += c;
        }
    }
    return result;
}

int main() {
    std::string ciphertext = "SPYFZAMPIFRY WU ZRC USWCVSC AH UCSEPWVM WVHAPQIZWAV NY ZPIVUHAPQWVM WZ WVZA I HAPQ ZRIZ WU XWHHWSELZ ZA EVXCPUZIVX OWZRAEZ ZRC FPAFCP GCY. SLIUUWSIL SWFRCPU UESR IU ZRC IHHWVC SWFRCP FPAJWXC I UWQFLC WVZPAXESZWAV ZA ZRC SAVSCFZU AH CVSPYFZWAV IVX XCSPYFZWAV.";

    std::cout << "--- Affine Cipher Decryption (a=5, b=8) ---\n";
    std::cout << decrypt(ciphertext, 5, 8) << "\n";
    return 0;
}
