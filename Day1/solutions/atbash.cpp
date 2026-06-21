#include <iostream>
#include <string>
#include <cctype>

std::string decrypt(const std::string& text) {
    std::string result = "";
    for (char c : text) {
        if (std::isalpha(c)) {
            char base = std::isupper(c) ? 'A' : 'a';
            result += base + (25 - (c - base));
        } else {
            result += c;
        }
    }
    return result;
}

int main() {
    std::string ciphertext = "Xibkgltizksb rh gsv hxrvmxv lu hvxfirmt rmulinzgrlm yb gizmhulinrmt rg rmgl z ulin gszg rh wruurxfog gl fmwvihgzmw drgslfg gsv kilkvi pvb. Xozhhrxzo xrksvih hfxs zh gsv Zgyzhs xrksvi kilerwv z hrnkov rmgilwfxgrlm gl gsv xlmxvkgh lu vmxibkgrlm zmw wvxibkgrlm. Zogslfts rg luuvih orggov kizxgrxzo hvxfirgb, rg ivnzrmh zm rnkligzmg vwfxzgrlmzo gllo uli hgfwbrmt gsv srhglib zmw velofgrlm lu xibkgltizksrx gvxsmrjfvh.";

    std::cout << "--- Atbash Cipher Decryption ---\n";
    std::cout << decrypt(ciphertext) << "\n";
    return 0;
}
