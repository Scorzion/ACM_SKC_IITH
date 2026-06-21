#include <iostream>
#include <string>
#include <vector>
#include <cmath>

std::string encrypt(const std::string& text, int rows) {
    int L = text.length();
    int cols = (L + rows - 1) / rows;
    std::string result = "";
    
    for (int c = 0; c < cols; ++c) {
        for (int r = 0; r < rows; ++r) {
            int idx = r * cols + c;
            if (idx < L) {
                result += text[idx];
            }
        }
    }
    return result;
}

std::string decrypt(const std::string& text, int rows) {
    int L = text.length();
    int cols = (L + rows - 1) / rows;
    
    // Determine the number of elements in each column
    int full_cols = L - (rows - 1) * cols;
    std::vector<int> col_sizes(cols, rows - 1);
    for (int i = 0; i < full_cols; ++i) {
        col_sizes[i] = rows;
    }
    
    // Distribute ciphertext chars to columns
    std::vector<std::string> grid(cols);
    int idx = 0;
    for (int c = 0; c < cols; ++c) {
        grid[c] = text.substr(idx, col_sizes[c]);
        idx += col_sizes[c];
    }
    
    // Read grid row-by-row
    std::string result = "";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (r < grid[c].length()) {
                result += grid[c][r];
            }
        }
    }
    return result;
}

int main() {
    std::string plaintext = "HELLOWORLD";
    std::cout << "--- Scytale Cipher (Plaintext: " << plaintext << ") ---\n";

    for (int key : {3, 4}) {
        std::string ct = encrypt(plaintext, key);
        std::string pt = decrypt(ct, key);
        std::cout << "Key (Rows): " << key << "\n";
        std::cout << "  Ciphertext: " << ct << "\n";
        std::cout << "  Decrypted:  " << pt << "\n\n";
    }
    return 0;
}
