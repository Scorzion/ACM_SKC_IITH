"""Slide-based differential attack on 4-round DES (Omega1 + Omega2)."""
import collections
import os
import random
import sys

sys.path.insert(0, os.path.abspath('..'))
from oracle_app.custom_DES import (
    S_BOXES, E, P, PC1, PC2, SHIFT_SCHEDULE,
    des_encrypt_hex, permute, left_shift, to_bin,
)

OMEGA1 = int('2000000000000000', 16)
OMEGA2 = int('0222222200000000', 16)
NUM_PAIRS = int(os.environ.get('NUM_PAIRS', '40'))
random.seed(42)

def bits_of(v, n):
    return [(v >> (n - 1 - i)) & 1 for i in range(n)]

def from_bits(bits):
    v = 0
    for b in bits:
        v = (v << 1) | b
    return v

def expand(R):
    rb = bits_of(R, 32)
    return from_bits([rb[E[k] - 1] for k in range(48)])

def permP(x):
    xb = bits_of(x, 32)
    return from_bits([xb[P[k] - 1] for k in range(32)])

Pinv = [0] * 32
for i in range(32):
    Pinv[P[i] - 1] = i + 1

def permPinv(y):
    yb = bits_of(y, 32)
    return from_bits([yb[Pinv[k] - 1] for k in range(32)])

def sbox(idx, six):
    b = bits_of(six, 6)
    row = (b[0] << 1) | b[5]
    col = (b[1] << 3) | (b[2] << 2) | (b[3] << 1) | b[4]
    return S_BOXES[idx][row][col]

BPRIME_CANDS = [permP(s1_out << 28) for s1_out in range(16)]

def check_expansion(label, L0p):
    ed = bits_of(expand(L0p), 48)
    print(f"{label} L0'=0x{L0p:08X}  E(L0') per S-box:")
    active = []
    for m in range(8):
        chunk = from_bits(ed[6 * m: 6 * m + 6])
        print(f'  S{m+1}: 0x{chunk:02x}', end='')
        if chunk:
            active.append(m + 1)
            print('  ACTIVE', end='')
        print()
    print(f'  -> active S-boxes: {active}')

def ct_halves(C, Cs):
    R4, L4 = C >> 32, C & 0xFFFFFFFF
    R4s, L4s = Cs >> 32, Cs & 0xFFFFFFFF
    return R4 ^ R4s, L4 ^ L4s, L4, L4s

def add_votes(C, Cs, Bprime, boxes, counts):
    slide_l, slide_r, L4, L4s = ct_halves(C, Cs)
    Dprime = Bprime ^ slide_l
    sob = bits_of(permPinv(Dprime), 32)
    idb = bits_of(expand(slide_r), 48)
    in_a = bits_of(expand(L4), 48)
    in_b = bits_of(expand(L4s), 48)
    for m in boxes:
        din = from_bits(idb[6 * m: 6 * m + 6])
        if din == 0:
            continue
        dout = from_bits(sob[4 * m: 4 * m + 4])
        a = from_bits(in_a[6 * m: 6 * m + 6])
        b = from_bits(in_b[6 * m: 6 * m + 6])
        for k in range(64):
            if sbox(m, a ^ k) ^ sbox(m, b ^ k) == dout:
                counts[m][k] += 1

def vote_margin(counts, boxes):
    margin = 0
    for m in boxes:
        top = counts[m].most_common(2)
        if not top:
            continue
        margin += top[0][1] - (top[1][1] if len(top) > 1 else 0)
    return margin

def rotr_list(bits, n):
    return bits[-n:] + bits[:-n]

TOTAL_SHIFT = sum(SHIFT_SCHEDULE)

def subkeys_from_combined0(c0):
    left, right = c0[:28], c0[28:]
    subs = []
    for shift in SHIFT_SCHEDULE:
        left = left_shift(left, shift)
        right = left_shift(right, shift)
        subs.append(permute(left + right, PC2))
    return subs

def encrypt_with_combined0(pt64, c0):
    subs = subkeys_from_combined0(c0)
    block = [str(b) for b in bits_of(pt64, 64)]
    left, right = block[:32], block[32:]
    for sk in subs:
        exp = permute(right, E)
        xr = [str(int(exp[i]) ^ int(sk[i])) for i in range(48)]
        sub = []
        for i in range(8):
            ch = xr[i * 6:(i + 1) * 6]
            r = int(ch[0] + ch[5], 2)
            c = int(''.join(ch[1:5]), 2)
            sub.extend(list(to_bin(S_BOXES[i][r][c], 4)))
        f_out = permute(sub, P)
        right, left = [str(int(left[i]) ^ int(f_out[i])) for i in range(32)], right
    return int(''.join(right + left), 2)

def invert_schedule(k4_int, encrypt_fn):
    k4_bits = bits_of(k4_int, 48)
    combined4 = [None] * 56
    for k in range(48):
        combined4[PC2[k] - 1] = k4_bits[k]
    unknown_pos = [i for i in range(56) if combined4[i] is None]
    kp_pt = random.getrandbits(64)
    kp_ct = encrypt_fn(kp_pt)
    for guess in range(1 << len(unknown_pos)):
        c4 = list(combined4)
        for idx, pos in enumerate(unknown_pos):
            c4[pos] = (guess >> (len(unknown_pos) - 1 - idx)) & 1
        c0 = rotr_list(c4[:28], TOTAL_SHIFT) + rotr_list(c4[28:], TOTAL_SHIFT)
        c0 = [str(b) for b in c0]
        if encrypt_with_combined0(kp_pt, c0) == kp_ct:
            key64 = [0] * 64
            for k in range(56):
                key64[PC1[k] - 1] = int(c0[k])
            key56 = [str(key64[i]) for i in range(64) if (i + 1) % 8 != 0]
            return f"{int(''.join(key56), 2):014X}"
    return None

def run_attack(encrypt_fn):
    o1 = {B: [collections.Counter() for _ in range(8)] for B in BPRIME_CANDS}
    o2 = {B: [collections.Counter() for _ in range(8)] for B in BPRIME_CANDS}
    for _ in range(NUM_PAIRS):
        pt = random.getrandbits(64)
        C1, Cs1 = encrypt_fn(pt), encrypt_fn(pt ^ OMEGA1)
        for B in BPRIME_CANDS:
            add_votes(C1, Cs1, B, list(range(1, 8)), o1[B])
        pt2 = random.getrandbits(64)
        C2, Cs2 = encrypt_fn(pt2), encrypt_fn(pt2 ^ OMEGA2)
        for B in BPRIME_CANDS:
            add_votes(C2, Cs2, B, [0], o2[B])
    B1 = max(BPRIME_CANDS, key=lambda B: vote_margin(o1[B], list(range(1, 8))))
    B2 = max(BPRIME_CANDS, key=lambda B: vote_margin(o2[B], [0]))
    chunks = [0] * 8
    chunks[0] = o2[B2][0].most_common(1)[0][0]
    for m in range(1, 8):
        if m == 3:
            continue
        chunks[m] = o1[B1][m].most_common(1)[0][0]
    key_hex = None
    k4_rec = None
    for s4 in range(64):
        chunks[3] = s4
        k4 = 0
        for m in range(8):
            k4 = (k4 << 6) | chunks[m]
        key_hex = invert_schedule(k4, encrypt_fn)
        if key_hex:
            k4_rec = k4
            break
    return B1, B2, k4_rec, key_hex

if __name__ == '__main__':
    print('=== expansion check ===')
    check_expansion('Omega1', OMEGA1 >> 32)
    check_expansion('Omega2', OMEGA2 >> 32)
    print("\nSlide map: l'=R4' (left CT diff), r'=L4' (right CT diff), D'=B' XOR l'.")
    KEY_HEX = open('../.env').read().strip().split('=')[1]
    q = 0

    def encrypt(pt64):
        global q
        q += 1
        return int(des_encrypt_hex(f'{pt64:016X}', KEY_HEX), 16)

    B1, B2, k4, key = run_attack(encrypt)
    ok = key and key.upper() == KEY_HEX.upper()
    print(f"\nNUM_PAIRS={NUM_PAIRS}")
    print(f"Omega1 best B'=0x{B1:08X}")
    print(f"Omega2 best B'=0x{B2:08X}")
    print(f"Recovered K4={k4:012X}" if k4 else 'K4 failed')
    print(f"Recovered KEY={key} expected={KEY_HEX} -> {'OK' if ok else 'FAIL'}")
    print(f'Total encrypt queries={q}')
    sys.exit(0 if ok else 1)
