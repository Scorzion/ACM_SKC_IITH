#include <iostream>
#include <string>

int modInverse(int a, int m) {
    for (int x = 1; x < m; ++x) {
        if ((a * x) % m == 1) return x;
    }
    return -1;
}

std::string encrypt(const std::string& text, const int K[2][2]) {
    std::string result = "";
    for (size_t i = 0; i < text.length(); i += 2) {
        int p0 = text[i] - 'A';
        int p1 = text[i+1] - 'A';
        int c0 = (K[0][0] * p0 + K[0][1] * p1) % 26;
        int c1 = (K[1][0] * p0 + K[1][1] * p1) % 26;
        result += (c0 + 'A');
        result += (c1 + 'A');
    }
    return result;
}

std::string decrypt(const std::string& text, const int K[2][2]) {
    int det = (K[0][0] * K[1][1] - K[0][1] * K[1][0]) % 26;
    if (det < 0) det += 26;
    int det_inv = modInverse(det, 26);
    if (det_inv == -1) return "Error: Matrix not invertible.";

    int K_inv[2][2];
    K_inv[0][0] = (K[1][1] * det_inv) % 26;
    K_inv[0][1] = (-K[0][1] * det_inv) % 26;
    K_inv[1][0] = (-K[1][0] * det_inv) % 26;
    K_inv[1][1] = (K[0][0] * det_inv) % 26;

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            K_inv[i][j] = (K_inv[i][j] + 26) % 26;
        }
    }

    std::string result = "";
    for (size_t i = 0; i < text.length(); i += 2) {
        int c0 = text[i] - 'A';
        int c1 = text[i+1] - 'A';
        int p0 = (K_inv[0][0] * c0 + K_inv[0][1] * c1) % 26;
        int p1 = (K_inv[1][0] * c0 + K_inv[1][1] * c1) % 26;
        result += (p0 + 'A');
        result += (p1 + 'A');
    }
    return result;
}

int main() {
    int K[2][2] = { {3, 3}, {2, 5} };
    std::string plaintext = "HELP";
    
    std::cout << "--- Hill Cipher (Plaintext: " << plaintext << ") ---\n";
    std::string ct = encrypt(plaintext, K);
    std::string pt = decrypt(ct, K);
    std::cout << "Ciphertext: " << ct << "\n";
    std::cout << "Decrypted:  " << pt << "\n";
    return 0;
}
