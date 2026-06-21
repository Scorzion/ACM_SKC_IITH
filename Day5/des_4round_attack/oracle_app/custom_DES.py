"""4-round DES variant used by the assignment oracle.

This uses the *standard* DES S-boxes (FIPS PUB 46) together with the standard
E, P, PC-1 and PC-2 tables. The cipher is reduced to 4 rounds and the initial
and final permutations (IP / IP^-1) are intentionally omitted. IP/IP^-1 are
fixed, key-independent bit permutations, so they do not affect a differential
attack; dropping them keeps the analysis focused on the round function while
remaining faithful to real DES.
"""

# Standard DES S-boxes (FIPS PUB 46)
S_BOXES = [
    # S1
    [[14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7],
     [0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8],
     [4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0],
     [15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13]],

    # S2
    [[15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10],
     [3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5],
     [0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15],
     [13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9]],

    # S3
    [[10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8],
     [13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1],
     [13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7],
     [1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12]],

    # S4
    [[7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15],
     [13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9],
     [10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4],
     [3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14]],

    # S5
    [[2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9],
     [14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6],
     [4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14],
     [11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3]],

    # S6
    [[12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11],
     [10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8],
     [9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6],
     [4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13]],

    # S7
    [[4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1],
     [13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6],
     [1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2],
     [6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12]],

    # S8
    [[13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7],
     [1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2],
     [7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8],
     [2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11]],
]

# Expansion permutation (E) table
E = [
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1,
]

# Permutation (P) table
P = [
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25,
]

# Key permutation (PC-1) table
PC1 = [
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4,
]

# Key permutation (PC-2) table
PC2 = [
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32,
]

# Left shift schedule for the four rounds
SHIFT_SCHEDULE = [1, 1, 2, 2]


def to_bin(value: int, length: int) -> str:
    """Return the binary representation of value padded to length bits."""
    return bin(value)[2:].zfill(length)


def permute(bits: list[str], table: list[int]) -> list[str]:
    """Permute bits according to table (1-indexed)."""
    return [bits[i - 1] for i in table]


def left_shift(bits: list[str], n: int) -> list[str]:
    """Rotate bits left by n positions."""
    return bits[n:] + bits[:n]


def hex_to_bits(hex_string: str) -> str:
    """Convert a 16-character hex string to a 64-bit binary string."""
    if len(hex_string) != 16:
        raise ValueError("hex input must be exactly 16 characters (64 bits).")
    return ''.join(bin(int(ch, 16))[2:].zfill(4) for ch in hex_string.upper())


def bits_to_hex(bit_string: str) -> str:
    """Convert a 64-bit binary string to a 16-character uppercase hex string."""
    if len(bit_string) != 64 or any(ch not in "01" for ch in bit_string):
        raise ValueError("bit string must be exactly 64 characters of '0' or '1'.")
    return ''.join(f"{int(bit_string[i:i + 4], 2):X}" for i in range(0, 64, 4))


def _normalize_bits(value: "str | list[str]", length: int, label: str) -> list[str]:
    """Ensure value is a list of '0'/'1' strings of the desired length."""
    if isinstance(value, list):
        bits = value
    elif isinstance(value, str):
        bits = list(value)
    else:
        raise TypeError(f"{label} must be a str or list of bit characters.")

    if len(bits) != length or any(bit not in {"0", "1"} for bit in bits):
        raise ValueError(f"{label} must be a {length}-bit sequence.")
    return bits


def generate_subkeys(key_bits: "str | list[str]") -> list[list[str]]:
    """Generate four 48-bit subkeys from the 64-bit key (with parity bits)."""
    key = _normalize_bits(key_bits, 64, "key")
    permuted_key = permute(key, PC1)
    left = permuted_key[:28]
    right = permuted_key[28:]

    subkeys: list[list[str]] = []
    for shift in SHIFT_SCHEDULE:
        left = left_shift(left, shift)
        right = left_shift(right, shift)
        combined = left + right
        subkeys.append(permute(combined, PC2))
    return subkeys


def s_box_substitution(bits: list[str]) -> list[str]:
    """Apply the eight standard DES S-boxes to 48 input bits."""
    result: list[str] = []
    for box_index in range(8):
        chunk = bits[box_index * 6:(box_index + 1) * 6]
        row = int(chunk[0] + chunk[5], 2)
        col = int(''.join(chunk[1:5]), 2)
        s_value = S_BOXES[box_index][row][col]
        result.extend(list(to_bin(s_value, 4)))
    return result


def feistel(right: list[str], subkey: list[str]) -> list[str]:
    """Feistel round function F(R, K)."""
    expanded = permute(right, E)
    xor_result = [str(int(expanded[i]) ^ int(subkey[i])) for i in range(48)]
    substituted = s_box_substitution(xor_result)
    return permute(substituted, P)


def des_encrypt(block_bits: "str | list[str]", key_bits: "str | list[str]") -> str:
    """Encrypt a 64-bit block using the 4-round DES variant (no IP / IP^-1)."""
    block = _normalize_bits(block_bits, 64, "plaintext")
    subkeys = generate_subkeys(key_bits)

    left = block[:32]
    right = block[32:]
    for subkey in subkeys:
        f_out = feistel(right, subkey)
        new_right = [str(int(left[i]) ^ int(f_out[i])) for i in range(32)]
        left, right = right, new_right

    return ''.join(right + left)


def des_decrypt(block_bits: "str | list[str]", key_bits: "str | list[str]") -> str:
    """Decrypt a 64-bit block with the 4-round DES variant (no IP / IP^-1)."""
    block = _normalize_bits(block_bits, 64, "ciphertext")
    subkeys = list(reversed(generate_subkeys(key_bits)))

    left = block[:32]
    right = block[32:]
    for subkey in subkeys:
        f_out = feistel(right, subkey)
        new_right = [str(int(left[i]) ^ int(f_out[i])) for i in range(32)]
        left, right = right, new_right

    return ''.join(right + left)


def des_encrypt_hex(plaintext_hex: str, key_hex: str) -> str:
    """Encrypt a hex-encoded block with a hex-encoded key.

    The key is supplied as 14 hex characters (56 bits). Parity bits are
    inserted as 0 after every 7 key bits to form the 64-bit key expected by
    PC-1 (the parity bits are discarded by PC-1, so their value is irrelevant).
    """
    key_bits_64 = _key56_hex_to_bits64(key_hex)
    return bits_to_hex(des_encrypt(hex_to_bits(plaintext_hex), key_bits_64))


def des_decrypt_hex(ciphertext_hex: str, key_hex: str) -> str:
    """Decrypt a hex-encoded block with a hex-encoded key (14 hex chars)."""
    key_bits_64 = _key56_hex_to_bits64(key_hex)
    return bits_to_hex(des_decrypt(hex_to_bits(ciphertext_hex), key_bits_64))


def _key56_hex_to_bits64(key_hex: str) -> str:
    """Expand a 14-hex-character (56-bit) key into a 64-bit key string.

    A 0 parity bit is appended after each group of 7 bits. PC-1 ignores the
    parity bits, so this is consistent with using the 56-bit effective key.
    """
    key_hex = key_hex.strip().upper()
    if len(key_hex) != 14 or any(ch not in "0123456789ABCDEF" for ch in key_hex):
        raise ValueError("key must be exactly 14 hexadecimal characters (56 bits).")
    key_bits_56 = to_bin(int(key_hex, 16), 56)
    key_bits_64 = []
    for i in range(8):
        key_bits_64.extend(list(key_bits_56[i * 7:(i + 1) * 7]))
        key_bits_64.append('0')
    return ''.join(key_bits_64)


if __name__ == "__main__":
    sample_key_hex = "3A5B7C9D2E4F6A"   # 14 hex chars = 56-bit key
    sample_pt_hex = "0123456789ABCDEF"   # 16 hex chars = 64-bit block

    ct_hex = des_encrypt_hex(sample_pt_hex, sample_key_hex)
    recovered_pt_hex = des_decrypt_hex(ct_hex, sample_key_hex)

    print("Plaintext :", sample_pt_hex)
    print("Ciphertext:", ct_hex)
    print("Decrypted :", recovered_pt_hex)

    if recovered_pt_hex == sample_pt_hex:
        print("Encryption/Decryption round-trip OK")
    else:
        print("Round-trip FAILED")
