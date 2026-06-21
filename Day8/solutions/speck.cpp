#include <iostream>
#include <iomanip>
#include <random>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

uint16_t ror(uint16_t x, int n) {
    return (x >> n) | (x << (16 - n));
}

uint16_t rol(uint16_t x, int n) {
    return (x << n) | (x >> (16 - n));
}

void r(uint16_t &x, uint16_t &y, uint16_t k) {
    x = (ror(x, 7) + y) ^ k;
    y = rol(y, 2) ^ x;
}

void ir(uint16_t &x, uint16_t &y, uint16_t k) {
    y = ror(y ^ x, 2);
    x = rol((x ^ k) - y, 7);
}

void ks(uint16_t K[4], uint16_t rk[22]) {
    uint16_t l[24];
    l[0] = K[1];
    l[1] = K[2];
    l[2] = K[3];
    rk[0] = K[0];
    for (int i = 0; i < 21; i++) {
        l[i + 3] = (rk[i] + ror(l[i], 7)) ^ i;
        rk[i + 1] = rol(rk[i], 2) ^ l[i + 3];
    }
}

void enc(uint16_t &x, uint16_t &y, uint16_t rk[22], int rd) {
    for (int i = 0; i < rd; i++) r(x, y, rk[i]);
}

void dec(uint16_t &x, uint16_t &y, uint16_t rk[22], int rd) {
    for (int i = rd - 1; i >= 0; i--) ir(x, y, rk[i]);
}

int main() {
    uint16_t K[4] = {0x0100, 0x0908, 0x1110, 0x1918};
    uint16_t rk[22];
    ks(K, rk);
    uint16_t tx = 0x6574, ty = 0x694c;
    enc(tx, ty, rk, 22);
    if (tx == 0xa868 && ty == 0x42f2) {
        cout << "Test passed" << endl;
    } else {
        cout << "Test failed" << endl;
        return 1;
    }
    dec(tx, ty, rk, 22);
    if (tx == 0x6574 && ty == 0x694c) {
        cout << "Decryption passed" << endl;
    } else {
        cout << "Decryption failed" << endl;
        return 1;
    }

    mt19937 rng(1337);
    int n = 10000000;
    
    for (int rd = 1; rd <= 5; rd++) {
        unordered_map<uint32_t, int> counts;
        for (int i = 0; i < n; i++) {
            uint16_t k0 = rng() & 0xffff;
            uint16_t k1 = rng() & 0xffff;
            uint16_t k2 = rng() & 0xffff;
            uint16_t k3 = rng() & 0xffff;
            uint16_t cur_K[4] = {k0, k1, k2, k3};
            uint16_t cur_rk[22];
            ks(cur_K, cur_rk);
            
            uint16_t p0x = rng() & 0xffff;
            uint16_t p0y = rng() & 0xffff;
            uint16_t p1x = p0x ^ 0x0040;
            uint16_t p1y = p0y;
            
            enc(p0x, p0y, cur_rk, rd);
            enc(p1x, p1y, cur_rk, rd);
            
            uint32_t diff = ((uint32_t)(p0x ^ p1x) << 16) | (p0y ^ p1y);
            counts[diff]++;
        }
        
        vector<pair<uint32_t, int>> sorted_counts(counts.begin(), counts.end());
        sort(sorted_counts.begin(), sorted_counts.end(), [](const pair<uint32_t, int>& a, const pair<uint32_t, int>& b) {
            return a.second > b.second;
        });
        
        cout << "Rounds: " << rd << endl;
        for (int i = 0; i < min(3, (int)sorted_counts.size()); i++) {
            uint32_t diff = sorted_counts[i].first;
            double prob = (double)sorted_counts[i].second / n;
            cout << "  Diff: " << setfill('0') << setw(4) << hex << (diff >> 16) << "/" 
                 << setfill('0') << setw(4) << hex << (diff & 0xffff) 
                 << " Prob: " << dec << prob << endl;
        }
    }
    return 0;
}
