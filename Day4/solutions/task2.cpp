#include <iostream>
#include <vector>
#include <cstdint>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <random>

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
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};
static const int P[32] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25};

static inline int par(uint32_t v) { return __builtin_parity(v); }

static inline int ext_r(int x) {
    return ((x >> 5) & 1) << 1 | (x & 1);
}

static inline int ext_c(int x) {
    return (x >> 1) & 0xF;
}

static inline int dot(int v, int m, int bits) {
    int r = 0;
    for (int i = 0; i < bits; ++i)
        r ^= ((v & m) >> i) & 1;
    return r;
}

static inline int sbox(int b, int x) { return S[b][ext_r(x)][ext_c(x)]; }

static uint64_t exp(uint32_t R) {
    uint64_t o = 0;
    for (int i = 0; i < 48; ++i) {
        int s = E[i] - 1;
        o |= ((uint64_t)((R >> (31 - s)) & 1) << (47 - i));
    }
    return o;
}

static uint32_t pP(uint32_t in) {
    uint32_t o = 0;
    for (int i = 0; i < 32; ++i) {
        int s = P[i] - 1;
        o |= (((in >> (31 - s)) & 1) << (31 - i));
    }
    return o;
}

static uint32_t F(uint32_t R, uint64_t K) {
    uint64_t x = exp(R) ^ K;
    uint32_t s = 0;
    for (int b = 0; b < 8; ++b) {
        int sh = 42 - b * 6;
        s |= (sbox(b, (x >> sh) & 0x3F) << (28 - b * 4));
    }
    return pP(s);
}

std::vector<std::vector<int>> lat(const int sbox[4][16]) {
    std::vector<std::vector<int>> l(64, std::vector<int>(16, 0));
    for (int x = 0; x < 64; ++x) {
        int r = ext_r(x), c = ext_c(x);
        int sx = sbox[r][c];
        for (int a = 0; a < 64; ++a) {
            int lp = dot(x, a, 6);
            for (int b = 0; b < 16; ++b) {
                int rp = dot(sx, b, 4);
                if (lp == rp) l[a][b]++;
            }
        }
    }
    for (int a = 0; a < 64; ++a)
        for (int b = 0; b < 16; ++b)
            l[a][b] -= 32;
    return l;
}

uint32_t a2R(int b, int alpha) {
    uint32_t m = 0;
    for (int j = 0; j < 6; ++j)
        if (alpha >> j & 1) {
            int pos = b * 6 + (5 - j);
            int r_bit = E[pos] - 1;
            m ^= (1u << (31 - r_bit));
        }
    return m;
}

uint32_t b2F(int b, int beta) {
    uint32_t m = 0;
    for (int k = 0; k < 4; ++k)
        if (beta >> k & 1) {
            int sl_bit = 3 + 4 * b - k;
            for (int i = 0; i < 32; ++i)
                if (P[i] - 1 == sl_bit) {
                    m ^= (1u << (31 - i));
                    break;
                }
        }
    return m;
}

std::vector<std::pair<int, int>> R2a(uint32_t rmask) {
    int alpha[8] = {0};
    for (int pos = 0; pos < 48; ++pos) {
        int r_bit = E[pos] - 1;
        if (rmask >> (31 - r_bit) & 1) {
            int b = pos / 6;
            int j = 5 - (pos % 6);
            alpha[b] ^= (1 << j);
        }
    }
    std::vector<std::pair<int, int>> res;
    for (int b = 0; b < 8; ++b)
        if (alpha[b]) res.push_back({b, alpha[b]});
    return res;
}

struct St {
    std::vector<std::pair<int, int>> active_alpha;
    std::vector<std::pair<int, int>> active_beta;
    uint32_t in_mask;
    uint32_t out_mask;
    double eps;
    bool valid;
};

St step(uint32_t in_mask, const std::vector<std::vector<std::vector<int>>> &all_lats) {
    St s;
    s.in_mask = in_mask;
    s.active_alpha = R2a(in_mask);
    s.valid = true;
    s.out_mask = 0;
    if (s.active_alpha.empty()) {
        s.eps = 0.5;
        return s;
    }
    double prod = 1.0;
    int k = s.active_alpha.size();
    for (const auto &p : s.active_alpha) {
        int b = p.first, alpha = p.second;
        int best_b = 0, max_bias = 0;
        for (int beta = 1; beta < 16; ++beta) {
            int bias = all_lats[b][alpha][beta];
            if (std::abs(bias) > std::abs(max_bias)) {
                max_bias = bias;
                best_b = beta;
            }
        }
        if (max_bias == 0) s.valid = false;
        s.active_beta.push_back({b, best_b});
        prod *= (double)max_bias / 64.0;
        s.out_mask ^= b2F(b, best_b);
    }
    s.eps = std::pow(2.0, k - 1) * prod;
    return s;
}

struct Tr {
    std::vector<St> steps;
    double eps_total;
    uint32_t gamma_P, gamma_U;
    bool valid;
};

Tr build(uint32_t r1_in, int rounds, const std::vector<std::vector<std::vector<int>>> &all_lats) {
    Tr t;
    t.valid = true;
    t.eps_total = 1.0;
    t.gamma_P = r1_in;
    uint32_t cur = r1_in;
    for (int r = 0; r < rounds; ++r) {
        St s = step(cur, all_lats);
        t.steps.push_back(s);
        if (!s.valid) t.valid = false;
        t.eps_total *= s.eps;
        cur = s.out_mask;
    }
    if (rounds > 0) t.eps_total *= std::pow(2.0, rounds - 1);
    else t.eps_total = 0.0;
    t.gamma_U = t.steps.empty() ? 0 : t.steps.back().out_mask;
    return t;
}

Tr search(int rounds, const std::vector<std::vector<std::vector<int>>> &all_lats) {
    Tr best;
    best.valid = false;
    best.eps_total = 0.0;
    for (int b = 0; b < 8; ++b) {
        for (int alpha = 1; alpha < 64; ++alpha) {
            uint32_t r1 = a2R(b, alpha);
            Tr t = build(r1, rounds, all_lats);
            if (t.valid) {
                if (!best.valid || std::abs(t.eps_total) > std::abs(best.eps_total))
                    best = t;
            }
        }
    }
    return best;
}

double verify(const Tr &t, int N = 500000, uint32_t seed = 99) {
    std::mt19937_64 rng(seed);
    int count = 0;
    for (int i = 0; i < N; ++i) {
        uint32_t L = rng(), R = rng();
        int tp = 0;
        for (size_t r = 0; r < t.steps.size(); ++r) {
            const auto &s = t.steps[r];
            uint32_t f_out = F(R, 0);
            tp ^= par(R & s.in_mask) ^ par(f_out & s.out_mask);
            uint32_t next_R = L ^ f_out;
            L = R; R = next_R;
        }
        if (tp == 0) count++;
    }
    return (double)count / N - 0.5;
}

void print(const Tr &t, const std::string &title) {
    std::cout << "\n" << title << "\n";
    for (size_t r = 0; r < t.steps.size(); ++r) {
        const auto &s = t.steps[r];
        std::cout << " Round " << (r + 1)
                  << ": in=0x" << std::hex << std::setw(8) << std::setfill('0') << s.in_mask
                  << " out=0x" << std::setw(8) << s.out_mask << std::dec << std::setfill(' ')
                  << "  active:";
        for (size_t i = 0; i < s.active_alpha.size(); ++i)
            std::cout << " S" << (s.active_alpha[i].first + 1)
                      << "(a=0x" << std::hex << s.active_alpha[i].second
                      << ",b=0x" << s.active_beta[i].second << std::dec << ")";
        std::cout << "  round_eps=" << std::fixed << std::setprecision(5) << s.eps << "\n";
    }
    std::cout << " Predicted total eps = " << t.eps_total << "  |eps| = " << std::abs(t.eps_total) << "\n";
    std::cout << " gamma_P = 0x" << std::hex << std::setw(8) << std::setfill('0') << t.gamma_P
              << "   gamma_U = 0x" << std::setw(8) << t.gamma_U << std::dec << std::setfill(' ') << "\n";
}

int main() {
    std::vector<std::vector<std::vector<int>>> all_lats(8);
    for (int b = 0; b < 8; ++b)
        all_lats[b] = lat(S[b]);
    for (int rounds = 1; rounds <= 3; ++rounds) {
        Tr t = search(rounds, all_lats);
        double e2 = t.eps_total * t.eps_total;
        std::cout << rounds << " " << std::fixed << std::setprecision(5) << std::abs(t.eps_total) << "\n";
    }
    Tr t1 = search(1, all_lats);
    print(t1, "Best valid 1-round trail");
    Tr t3 = search(3, all_lats);
    print(t3, "Best valid 3-round trail");
    std::cout << "empirical: " << verify(t3) << "\n";
    return 0;
}
