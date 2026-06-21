#include <iostream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <iomanip>

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

void blk(uint32_t st[16], uint8_t ks[64]) {
  uint32_t w[16];
  for (int i = 0; i < 16; i++) w[i] = st[i];
  for (int k = 0; k < 10; k++) {
    qr(w, 0, 4, 8, 12);
    qr(w, 5, 9, 13, 1);
    qr(w, 10, 14, 2, 6);
    qr(w, 15, 3, 7, 11);
    qr(w, 0, 1, 2, 3);
    qr(w, 5, 6, 7, 4);
    qr(w, 10, 11, 8, 9);
    qr(w, 15, 12, 13, 14);
  }
  for (int i = 0; i < 16; i++) {
    uint32_t v = w[i] + st[i];
    ks[i * 4] = v & 0xFF;
    ks[i * 4 + 1] = (v >> 8) & 0xFF;
    ks[i * 4 + 2] = (v >> 16) & 0xFF;
    ks[i * 4 + 3] = (v >> 24) & 0xFF;
  }
}

void setup(uint32_t st[16], const uint8_t k[32], const uint8_t nonce[8], uint64_t ctr) {
  st[0] = 0x61786520;
  st[5] = 0x6e642033;
  st[10] = 0x20627974;
  st[15] = 0x65206b65;
  for (int i = 0; i < 4; ++i) {
    st[i + 1] = k[i * 4] | (k[i * 4 + 1] << 8) | (k[i * 4 + 2] << 16) | (k[i * 4 + 3] << 24);
    st[i + 11] = k[i * 4 + 16] | (k[i * 4 + 17] << 8) | (k[i * 4 + 18] << 16) | (k[i * 4 + 19] << 24);
  }
  st[6] = nonce[0] | (nonce[1] << 8) | (nonce[2] << 16) | (nonce[3] << 24);
  st[7] = nonce[4] | (nonce[5] << 8) | (nonce[6] << 16) | (nonce[7] << 24);
  st[8] = ctr & 0xFFFFFFFF;
  st[9] = (ctr >> 32) & 0xFFFFFFFF;
}

void crypt(const uint8_t k[32], const uint8_t nonce[8], uint64_t ctr, const uint8_t *in, uint8_t *out, size_t len) {
  uint32_t st[16];
  uint8_t ks[64];
  size_t i = 0;
  while (len > 0) {
    setup(st, k, nonce, ctr++);
    blk(st, ks);
    size_t chunk = len > 64 ? 64 : len;
    for (size_t j = 0; j < chunk; ++j) {
      out[i + j] = in[i + j] ^ ks[j];
    }
    i += chunk;
    len -= chunk;
  }
}

int main() {
  uint8_t k[32] = {0};
  uint8_t nonce[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  for (int i = 0; i < 32; ++i) k[i] = i;
  string pt = "Hello Salsa20 Stream Cipher Encryption and Decryption!";
  size_t len = pt.size();
  vector<uint8_t> ct(len);
  vector<uint8_t> dec_pt(len);
  crypt(k, nonce, 0, (const uint8_t *)pt.data(), ct.data(), len);
  cout << "Ciphertext: ";
  for (uint8_t x : ct) cout << hex << setw(2) << setfill('0') << (int)x;
  cout << dec << "\n";
  crypt(k, nonce, 0, ct.data(), dec_pt.data(), len);
  string recovered((const char *)dec_pt.data(), len);
  cout << "Recovered : " << recovered << "\n";
  return 0;
}
