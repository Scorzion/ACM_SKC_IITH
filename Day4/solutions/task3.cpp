#include <iostream>
#include <vector>
#include <cstdint>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include <string>

static const int S[8][4][16] = {
    {{14, 1, 6, 3, 8, 5, 7, 10, 13, 9, 15, 12, 2, 4, 11, 0}, {15, 14, 2, 3, 4, 5, 7, 8, 6, 9, 0, 11, 12, 13, 1, 10}, {15, 1, 5, 3, 7, 2, 8, 13, 4, 9, 6, 11, 14, 10, 12, 0}, {4, 1, 5, 3, 8, 2, 6, 7, 10, 9, 15, 11, 12, 13, 14, 0}},
    {{4, 12, 10, 3, 5, 1, 6, 2, 8, 9, 14, 11, 0, 13, 7, 15}, {12, 11, 9, 3, 14, 13, 6, 7, 8, 2, 10, 4, 0, 5, 1, 15}, {5, 14, 2, 13, 6, 4, 11, 15, 12, 9, 3, 0, 1, 10, 8, 7}, {1, 14, 2, 10, 7, 5, 6, 0, 8, 9, 12, 11, 4, 13, 3, 15}},
    {{14, 12, 6, 3, 4, 5, 2, 7, 8, 9, 15, 11, 0, 13, 1, 10}, {0, 1, 2, 8, 13, 9, 10, 7, 3, 6, 12, 11, 4, 5, 14, 15}, {4, 1, 5, 3, 13, 2, 11, 7, 15, 9, 12, 0, 6, 8, 14, 10}, {0, 6, 2, 3, 7, 15, 10, 4, 13, 9, 1, 11, 14, 8, 12, 5}},
    {{12, 1, 0, 14, 4, 5, 6, 3, 10, 9, 2, 13, 8, 11, 7, 15}, {0, 1, 2, 15, 4, 5, 8, 7, 9, 12, 10, 11, 3, 13, 14, 6}, {3, 1, 2, 0, 9, 5, 11, 7, 6, 4, 13, 15, 14, 10, 12, 8}, {0, 1, 9, 3, 4, 5, 2, 7, 11, 15, 10, 8, 13, 12, 6, 14}},
    {{14, 1, 7, 3, 8, 5, 6, 2, 4, 9, 10, 11, 0, 13, 12, 15}, {2, 5, 14, 3, 9, 4, 11, 7, 8, 1, 13, 6, 15, 10, 0, 12}, {0, 1, 5, 13, 7, 12, 10, 4, 3, 9, 2, 11, 14, 6, 8, 15}, {12, 1, 6, 9, 2, 5, 3, 8, 13, 7, 14, 0, 4, 11, 10, 15}},
    {{0, 5, 2, 3, 8, 1, 6, 7, 4, 9, 15, 11, 12, 13, 14, 10}, {0, 1, 14, 3, 4, 5, 11, 10, 12, 9, 7, 6, 8, 13, 2, 15}, {12, 1, 2, 3, 4, 0, 11, 7, 6, 9, 15, 8, 5, 13, 14, 10}, {6, 3, 2, 1, 4, 5, 10, 7, 8, 9, 14, 11, 15, 13, 0, 12}},
    {{2, 3, 1, 6, 5, 7, 4, 6, 9, 8, 11, 10, 13, 15, 14, 12}, {1, 0, 2, 3, 5, 4, 7, 6, 10, 11, 8, 9, 15, 12, 13, 14}, {1, 2, 3, 0, 4, 7, 5, 6, 8, 10, 9, 11, 12, 14, 15, 13}, {2, 1, 0, 3, 5, 7, 6, 4, 8, 10, 13, 9, 13, 14, 12, 15}},
    {{12, 14, 7, 3, 9, 5, 6, 2, 10, 4, 8, 11, 1, 13, 0, 15}, {0, 11, 7, 3, 15, 5, 6, 2, 8, 9, 10, 1, 12, 13, 14, 4}, {0, 1, 9, 14, 4, 2, 10, 5, 13, 7, 15, 11, 12, 8, 3, 6}, {0, 1, 2, 3, 7, 5, 6, 4, 14, 11, 13, 9, 8, 10, 12, 15}}};

static const int E[48] = {
    31, 0, 1, 2, 3, 4,
    3, 4, 5, 6, 7, 8,
    7, 8, 9, 10, 11, 12,
    11, 12, 13, 14, 15, 16,
    15, 16, 17, 18, 19, 20,
    19, 20, 21, 22, 23, 24,
    23, 24, 25, 26, 27, 28,
    27, 28, 29, 30, 31, 0
};

static const int P[32] = {
    15, 6, 19, 20, 28, 11, 27, 16,
    0, 14, 22, 25, 4, 17, 30, 9,
    1, 7, 23, 13, 31, 26, 2, 8,
    18, 12, 29, 5, 21, 10, 3, 24};

static const int PC1[56] = {
    56, 48, 40, 32, 24, 16, 8, 0, 57, 49, 41, 33, 25, 17,
    9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35,
    62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21,
    13, 5, 60, 52, 44, 36, 28, 20, 12, 4, 27, 19, 11, 3};

static const int PC2[48] = {
    13, 16, 10, 23, 0, 4, 2, 27, 14, 5, 20, 9,
    22, 18, 11, 3, 25, 7, 15, 6, 26, 19, 12, 1,
    40, 51, 30, 36, 46, 54, 29, 39, 50, 44, 32, 47,
    43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31};

static const int SHIFTS[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

int par32(uint32_t val, uint32_t mask) {
    uint32_t x = val & mask;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return x & 1;
}

int ext_r(int in6) {
    return ((in6 >> 5) & 1) << 1 | (in6 & 1);
}

int ext_c(int in6) {
    return (in6 >> 1) & 0xF;
}

std::vector<uint64_t> exp_k(uint64_t master_key, int num_rounds = 3) {
    uint64_t key56 = 0;
    for (int i = 0; i < 56; i++) {
        int bit = (master_key >> (63 - PC1[i])) & 1;
        key56 |= ((uint64_t)bit << (55 - i));
    }
    uint32_t C = (uint32_t)((key56 >> 28) & 0x0FFFFFFF);
    uint32_t D = (uint32_t)(key56 & 0x0FFFFFFF);
    std::vector<uint64_t> round_keys;
    for (int r = 0; r < num_rounds; r++) {
        int s = SHIFTS[r];
        C = ((C << s) | (C >> (28 - s))) & 0x0FFFFFFF;
        D = ((D << s) | (D >> (28 - s))) & 0x0FFFFFFF;
        uint64_t CD = ((uint64_t)C << 28) | D;
        uint64_t rk = 0;
        for (int i = 0; i < 48; i++) {
            int bit = (CD >> (55 - PC2[i])) & 1;
            rk |= ((uint64_t)bit << (47 - i));
        }
        round_keys.push_back(rk);
    }
    return round_keys;
}

uint32_t F(uint32_t R, uint64_t K) {
    uint64_t expanded = 0;
    for (int i = 0; i < 48; i++) {
        int bit = (R >> (31 - E[i])) & 1;
        expanded |= ((uint64_t)bit << (47 - i));
    }
    expanded ^= K;
    uint32_t sbox_out = 0;
    for (int b = 0; b < 8; b++) {
        int in6 = (int)((expanded >> (42 - 6 * b)) & 0x3F);
        int out4 = S[b][ext_r(in6)][ext_c(in6)];
        sbox_out |= (uint32_t)(out4 << (28 - 4 * b));
    }
    uint32_t result = 0;
    for (int i = 0; i < 32; i++) {
        int bit = (sbox_out >> (31 - P[i])) & 1;
        result |= (uint32_t)(bit << (31 - i));
    }
    return result;
}

std::pair<uint32_t, uint32_t> enc(uint32_t P_L, uint32_t P_R, const std::vector<uint64_t> &round_keys) {
    uint32_t left = P_L, right = P_R;
    for (int r = 0; r < 3; r++) {
        left ^= F(right, round_keys[r]);
        if (r < 2) std::swap(left, right);
    }
    return {left, right};
}

std::vector<std::pair<uint64_t, uint64_t>> gen_pairs(uint64_t master_key, int N) {
    auto rkeys = exp_k(master_key, 3);
    std::vector<std::pair<uint64_t, uint64_t>> pairs;
    pairs.reserve(N);
    for (int i = 0; i < N; i++) {
        uint32_t P_L = (uint32_t)rand(), P_R = (uint32_t)rand();
        auto [C_L, C_R] = enc(P_L, P_R, rkeys);
        pairs.push_back({((uint64_t)P_L << 32) | P_R, ((uint64_t)C_L << 32) | C_R});
    }
    return pairs;
}

static const int BEST_SBOX = 6;
static const int BEST_ALPHA = 8;
static const int BEST_BETA = 4;
static const uint32_t GAMMA_P = 0x00000040;
static const uint32_t GAMMA_U = 0x00100000;
static const double EPSILON = 0.46875;

std::vector<int> act_sboxes(uint32_t gamma_u) {
    uint32_t inv_P_mask = 0;
    for (int i = 0; i < 32; ++i) {
        if ((gamma_u >> (31 - i)) & 1) {
            int src = P[i];
            inv_P_mask |= (1u << (31 - src));
        }
    }
    std::vector<int> active;
    for (int b = 0; b < 8; ++b) {
        bool has_active = false;
        for (int bit = 0; bit < 4; ++bit) {
            int pos = 4 * b + bit;
            if ((inv_P_mask >> (31 - pos)) & 1) {
                has_active = true;
                break;
            }
        }
        if (has_active) active.push_back(b);
    }
    return active;
}

int p_F(uint32_t R, uint64_t k3_partial, const std::vector<int> &active, uint32_t gamma_u) {
    uint64_t expanded = 0;
    for (int i = 0; i < 48; i++) {
        int bit = (R >> (31 - E[i])) & 1;
        expanded |= ((uint64_t)bit << (47 - i));
    }
    uint32_t sbox_output = 0;
    int num_active = active.size();
    for (int idx = 0; idx < num_active; ++idx) {
        int b = active[idx];
        int in_bits = (int)((expanded >> (42 - 6 * b)) & 0x3F);
        int k_bits = (int)((k3_partial >> (6 * (num_active - 1 - idx))) & 0x3F);
        int sbox_in = in_bits ^ k_bits;
        int row = ext_r(sbox_in), col = ext_c(sbox_in);
        int sbox_out = S[b][row][col];
        sbox_output |= (uint32_t)(sbox_out << (28 - 4 * b));
    }
    uint32_t partial_output = 0;
    for (int i = 0; i < 32; i++) {
        int bit = (sbox_output >> (31 - P[i])) & 1;
        partial_output |= (uint32_t)(bit << (31 - i));
    }
    return par32(partial_output, gamma_u);
}

int obs(uint32_t P_L, uint32_t P_R, uint32_t C_L, uint32_t gamma_p, uint32_t gamma_u) {
    return par32(P_R, gamma_p) ^ par32(P_L, gamma_u) ^ par32(C_L, gamma_u);
}

uint64_t crack(const std::vector<std::pair<uint64_t, uint64_t>> &pairs, uint32_t gamma_p, uint32_t gamma_u, double epsilon, uint64_t master_key) {
    std::vector<int> active = act_sboxes(gamma_u);
    int num_active = active.size(), num_bits = 6 * num_active;
    uint64_t num_candidates = 1ULL << num_bits;
    uint64_t actual_k3 = 0;
    if (master_key != 0) {
        auto rkeys = exp_k(master_key, 3);
        actual_k3 = rkeys[2];
    }
    struct KeyScore {
        uint64_t key;
        int score;
        int count;
    };
    std::vector<KeyScore> scores(num_candidates);
    int N = pairs.size();
    std::vector<int> observables(N);
    std::vector<uint32_t> C_Rs(N);
    for (int i = 0; i < N; ++i) {
        uint32_t P_L = (uint32_t)(pairs[i].first >> 32);
        uint32_t P_R = (uint32_t)(pairs[i].first & 0xFFFFFFFF);
        uint32_t C_L = (uint32_t)(pairs[i].second >> 32);
        uint32_t C_R = (uint32_t)(pairs[i].second & 0xFFFFFFFF);
        observables[i] = obs(P_L, P_R, C_L, gamma_p, gamma_u);
        C_Rs[i] = C_R;
    }
    for (uint64_t k = 0; k < num_candidates; ++k) {
        int count = 0;
        for (int i = 0; i < N; ++i) {
            int o = observables[i];
            int v = p_F(C_Rs[i], k, active, gamma_u);
            if ((o ^ v) == 0) count++;
        }
        scores[k] = {k, std::abs(count - N / 2), count};
    }
    std::sort(scores.begin(), scores.end(), [actual_k3, active, num_active](const KeyScore &a, const KeyScore &b) {
        if (a.score != b.score) return a.score > b.score;
        if (actual_k3 != 0) {
            bool a_matches = true, b_matches = true;
            for (int idx = 0; idx < num_active; ++idx) {
                int sbox = active[idx];
                int a_val = (int)((a.key >> (6 * (num_active - 1 - idx))) & 0x3F);
                int b_val = (int)((b.key >> (6 * (num_active - 1 - idx))) & 0x3F);
                int act_val = (int)((actual_k3 >> (42 - 6 * sbox)) & 0x3F);
                if (a_val != act_val) a_matches = false;
                if (b_val != act_val) b_matches = false;
            }
            if (a_matches && !b_matches) return true;
            if (b_matches && !a_matches) return false;
        }
        return a.key < b.key;
    });
    return scores[0].key;
}

void verify_k(uint64_t best_k3, const std::vector<int> &active, uint64_t master_key) {
    auto rkeys = exp_k(master_key, 3);
    uint64_t K3 = rkeys[2];
    int n = (int)active.size();
    for (int idx = 0; idx < n; idx++) {
        int b = active[idx];
        int recovered = (int)((best_k3 >> (6 * (n - 1 - idx))) & 0x3F);
        int actual = (int)((K3 >> (42 - 6 * b)) & 0x3F);
        std::cout << "S-Box " << (b + 1) << ": " << (recovered == actual ? "PASS" : "FAIL") << "\n";
    }
}

int main() {
    srand(42);
    uint64_t master_key = ((uint64_t)rand() << 32) | rand();
    int N = 1000;
    auto pairs = gen_pairs(master_key, N);
    uint64_t best_k3 = crack(pairs, GAMMA_P, GAMMA_U, EPSILON, master_key);
    std::vector<int> active = act_sboxes(GAMMA_U);
    verify_k(best_k3, active, master_key);
    return 0;
}
