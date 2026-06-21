#include <bits/stdc++.h>
using namespace std;

uint32_t rotl(uint32_t x, int n) {
  return (x << n) | (x >> (32 - n));
}

void qr(uint32_t s[16], int a, int b, int c, int d) {
  s[b] ^= rotl(s[a] + s[d], 7);
  s[c] ^= rotl(s[b] + s[a], 9);
  s[d] ^= rotl(s[c] + s[b], 13);
  s[a] ^= rotl(s[d] + s[c], 18);
}

void blk(uint32_t st[16]) {
  uint32_t w[16];
  for (int i = 0; i < 16; i++) w[i] = st[i];
  for (int k = 0; k < 2; k++) {
    qr(w, 0, 4, 8, 12);
    qr(w, 5, 9, 13, 1);
    qr(w, 10, 14, 2, 6);
    qr(w, 15, 3, 7, 11);
    qr(w, 0, 1, 2, 3);
    qr(w, 5, 6, 7, 4);
    qr(w, 10, 11, 8, 9);
    qr(w, 15, 12, 13, 14);
  }
  for (int i = 0; i < 16; i++) st[i] = w[i] + st[i];
}

int main() {
  mt19937 rng(42);
  long long total_hw = 0;
  const int n_hw = 100000;
  for (int t = 0; t < n_hw; ++t) {
    uint32_t st1[16], st2[16];
    for (int i = 0; i < 16; ++i) {
      st1[i] = rng();
      st2[i] = st1[i];
    }
    st2[7] ^= (1U << 31);
    qr(st1, 0, 4, 8, 12);
    qr(st1, 5, 9, 13, 1);
    qr(st1, 10, 14, 2, 6);
    qr(st1, 15, 3, 7, 11);
    qr(st2, 0, 4, 8, 12);
    qr(st2, 5, 9, 13, 1);
    qr(st2, 10, 14, 2, 6);
    qr(st2, 15, 3, 7, 11);
    int hw = 0;
    for (int i = 0; i < 16; ++i) {
      hw += __builtin_popcount(st1[i] ^ st2[i]);
    }
    total_hw += hw;
  }
  cout << "Avg Hamming Weight (1 round): " << (double)total_hw / n_hw << "\n";

  int cnt = 0;
  const int n_dp = 1 << 22;
  for (int t = 0; t < n_dp; ++t) {
    uint32_t st1[16], st2[16];
    for (int i = 0; i < 16; ++i) {
      st1[i] = rng();
      st2[i] = st1[i];
    }
    st2[7] ^= (1U << 31);
    blk(st1);
    blk(st2);
    if (((st1[1] ^ st2[1]) >> 14 & 1U) == 0) {
      cnt++;
    }
  }
  cout << "Differential Probability (diff=0): " << fixed << setprecision(4) << (double)cnt / n_dp << "\n";
  return 0;
}