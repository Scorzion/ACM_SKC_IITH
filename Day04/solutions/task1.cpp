#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

const int S[8][4][16] = {
    {{14, 1, 6, 3, 8, 5, 7, 10, 13, 9, 15, 12, 2, 4, 11, 0},
     {15, 14, 2, 3, 4, 5, 7, 8, 6, 9, 0, 11, 12, 13, 1, 10},
     {15, 1, 5, 3, 7, 2, 8, 13, 4, 9, 6, 11, 14, 10, 12, 0},
     {4, 1, 5, 3, 8, 2, 6, 7, 10, 9, 15, 11, 12, 13, 14, 0}},
    {{4, 12, 10, 3, 5, 1, 6, 2, 8, 9, 14, 11, 0, 13, 7, 15},
     {12, 11, 9, 3, 14, 13, 6, 7, 8, 2, 10, 4, 0, 5, 1, 15},
     {5, 14, 2, 13, 6, 4, 11, 15, 12, 9, 3, 0, 1, 10, 8, 7},
     {1, 14, 2, 10, 7, 5, 6, 0, 8, 9, 12, 11, 4, 13, 3, 15}},
    {{14, 12, 6, 3, 4, 5, 2, 7, 8, 9, 15, 11, 0, 13, 1, 10},
     {0, 1, 2, 8, 13, 9, 10, 7, 3, 6, 12, 11, 4, 5, 14, 15},
     {4, 1, 5, 3, 13, 2, 11, 7, 15, 9, 12, 0, 6, 8, 14, 10},
     {0, 6, 2, 3, 7, 15, 10, 4, 13, 9, 1, 11, 14, 8, 12, 5}},
    {{12, 1, 0, 14, 4, 5, 6, 3, 10, 9, 2, 13, 8, 11, 7, 15},
     {0, 1, 2, 15, 4, 5, 8, 7, 9, 12, 10, 11, 3, 13, 14, 6},
     {3, 1, 2, 0, 9, 5, 11, 7, 6, 4, 13, 15, 14, 10, 12, 8},
     {0, 1, 9, 3, 4, 5, 2, 7, 11, 15, 10, 8, 13, 12, 6, 14}},
    {{14, 1, 7, 3, 8, 5, 6, 2, 4, 9, 10, 11, 0, 13, 12, 15},
     {2, 5, 14, 3, 9, 4, 11, 7, 8, 1, 13, 6, 15, 10, 0, 12},
     {0, 1, 5, 13, 7, 12, 10, 4, 3, 9, 2, 11, 14, 6, 8, 15},
     {12, 1, 6, 9, 2, 5, 3, 8, 13, 7, 14, 0, 4, 11, 10, 15}},
    {{0, 5, 2, 3, 8, 1, 6, 7, 4, 9, 15, 11, 12, 13, 14, 10},
     {0, 1, 14, 3, 4, 5, 11, 10, 12, 9, 7, 6, 8, 13, 2, 15},
     {12, 1, 2, 3, 4, 0, 11, 7, 6, 9, 15, 8, 5, 13, 14, 10},
     {6, 3, 2, 1, 4, 5, 10, 7, 8, 9, 14, 11, 15, 13, 0, 12}},
    {{2, 3, 1, 6, 5, 7, 4, 6, 9, 8, 11, 10, 13, 15, 14, 12},
     {1, 0, 2, 3, 5, 4, 7, 6, 10, 11, 8, 9, 15, 12, 13, 14},
     {1, 2, 3, 0, 4, 7, 5, 6, 8, 10, 9, 11, 12, 14, 15, 13},
     {2, 1, 0, 3, 5, 7, 6, 4, 8, 10, 13, 9, 13, 14, 12, 15}},
    {{12, 14, 7, 3, 9, 5, 6, 2, 10, 4, 8, 11, 1, 13, 0, 15},
     {0, 11, 7, 3, 15, 5, 6, 2, 8, 9, 10, 1, 12, 13, 14, 4},
     {0, 1, 9, 14, 4, 2, 10, 5, 13, 7, 15, 11, 12, 8, 3, 6},
     {0, 1, 2, 3, 7, 5, 6, 4, 14, 11, 13, 9, 8, 10, 12, 15}}};

int ext_r(int in) {
  return (((in >> 5) & 1) << 1) | (in & 1);
}

int ext_c(int in) {
  return (in >> 1) & 0x0F;
}

int dot(int val, int mask, int bits) {
  int res = 0;
  int t = val & mask;
  for (int i = 0; i < bits; ++i) {
    res ^= (t >> i) & 1;
  }
  return res;
}

std::vector<std::vector<int>> lat(const int sbox[4][16]) {
  std::vector<std::vector<int>> l(64, std::vector<int>(16, 0));
  for (int x = 0; x < 64; x++) {
    int r = ext_r(x);
    int c = ext_c(x);
    int sx = sbox[r][c];
    for (int a = 0; a < 64; ++a) {
      int lp = dot(x, a, 6);
      for (int b = 0; b < 16; ++b) {
        if (lp == dot(sx, b, 4)) {
          l[a][b]++;
        }
      }
    }
  }
  for (int a = 0; a < 64; ++a) {
    for (int b = 0; b < 16; ++b) {
      l[a][b] -= 32;
    }
  }
  return l;
}

int max_b(const std::vector<std::vector<int>> &l, int &ba, int &bb) {
  int mx = 0;
  ba = 0;
  bb = 0;
  for (int a = 0; a < 64; ++a) {
    for (int b = 0; b < 16; ++b) {
      if (a == 0 && b == 0) continue;
      int bias = std::abs(l[a][b]);
      if (bias > mx) {
        mx = bias;
        ba = a;
        bb = b;
      }
    }
  }
  return mx;
}

void print(const std::vector<std::vector<int>> &l, int idx) {
  std::cout << "\n Linear Approximation Table  --  S-Box " << idx + 1 << "\n";
  std::cout << " Alpha \\ Beta |";
  for (int b = 0; b < 16; ++b) {
    std::cout << std::setw(4) << std::hex << b << std::dec;
  }
  std::cout << "\n";
  for (int a = 0; a < 64; ++a) {
    std::cout << std::setw(12) << a << "  |";
    for (int b = 0; b < 16; ++b) {
      std::cout << std::setw(4) << l[a][b];
    }
    std::cout << "\n";
  }
  int ba, bb;
  int mx = max_b(l, ba, bb);
  int raw = mx + 32;
  double eps = (double)l[ba][bb] / 64.0;
  std::cout << "\n Best non-trivial approximation:\n";
  std::cout << "   Input  mask alpha = " << ba << "  (0x" << std::hex << ba << std::dec << ")\n";
  std::cout << "   Output mask beta  = " << bb << "  (0x" << std::hex << bb << std::dec << ")\n";
  std::cout << "   Bias              = " << l[ba][bb] << "  (raw count = " << raw << " / 64)\n";
  std::cout << std::fixed << std::setprecision(4);
  std::cout << "   Epsilon (bias/64) = " << eps << "\n";
  std::cout << "   |Epsilon|         = " << std::abs(eps) << "\n";
}

int main() {
  for (int i = 0; i < 8; ++i) {
    auto l = lat(S[i]);
    print(l, i);
  }
  return 0;
}
