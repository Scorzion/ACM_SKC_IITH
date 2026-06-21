# Assignment: Differential Cryptanalysis of 4-Round DES

## Objective

Analyze and attack a **4-round DES** implementation using differential
cryptanalysis. Unlike a toy cipher, this variant uses the **standard DES
S-boxes** (FIPS PUB 46). You are provided with:

1. The S-boxes used by the cipher, as a text file (`S_BOXES.txt`) - these are
   the real DES S-boxes.
2. An encryption oracle that encrypts chosen plaintexts under a fixed secret
   key: `http://<SERVER_IP>/`. The same key is used for everyone.

Your task is to study the S-box differential properties, build a differential
characteristic across the rounds, recover the last round subkey, and from it
reconstruct the full key.

## The cipher

- Standard DES round function: expansion `E`, key mixing, 8 S-boxes, permutation
  `P`. Standard key schedule (`PC-1`, `PC-2`, left-shift schedule `[1, 1, 2, 2]`).
- **4 rounds** only.
- The initial and final permutations (`IP`, `IP`^-1) are **omitted**. They are
  fixed, key-independent bit permutations, so the 64-bit oracle input is fed
  straight into the Feistel network and the 64-bit output is `(R4 || L4)` with
  no final permutation. Account for this when you set up input differences.
- Inputs/outputs are 64-bit values written as **16 hexadecimal characters**.
- The key is a 56-bit DES key (parity bits omitted).

## Oracle interface

- Web form: `http://<SERVER_IP>/`
- JSON API: `POST http://<SERVER_IP>/api/encrypt` with body
  `{"plaintext": "0123456789ABCDEF"}`, returns `{"ciphertext": "..."}`.
- **Query budget:** up to **1000 queries per day** (per source IP). A differential
  attack on 4-round DES needs only a few hundred chosen-plaintext pairs, so this
  is ample - but plan your queries and cache results.

## Background

Differential cryptanalysis is a chosen-plaintext attack that exploits how an
input XOR difference propagates through the cipher with non-uniform probability.
The non-linearity of DES lives entirely in the S-boxes, so the attack is driven
by the S-box **difference distribution tables (DDTs)**.

## Tasks

### 1. Analyze the S-boxes
- Compute the DDT for each of the 8 S-boxes (a 64x16 table of input-XOR to
  output-XOR counts).
- Identify high-probability input/output differentials (e.g. the well-known
  entries with count 16 out of 64 for some S-boxes).

### 2. Build a differential characteristic
- Using `E`, the S-box differentials, and `P`, construct a characteristic over
  the first rounds whose output difference into the last round is predictable
  with good probability.
- A standard choice for 4-round DES uses a characteristic in which one round's
  F-output difference is zero (because the active right half has zero
  difference), giving a clean predicted difference entering the final round.
  Derive the probability of your characteristic.

### 3. Generate and filter pairs
- Choose plaintext pairs `(P, P*)` with your fixed input XOR `P' = P XOR P*`.
- Query the oracle for both, obtain `(C, C*)`, and compute `C' = C XOR C*`.
- Use the characteristic to predict the difference entering the last round's F
  function. Discard pairs that cannot satisfy it ("filtering"); the survivors
  are "right pairs".

### 4. Recover the last round subkey K4
- For each S-box, for every candidate 6-bit subkey segment, count how many
  filtered pairs are consistent with the expected S-box input/output difference.
- The correct subkey bits are suggested far more often than wrong ones. Recover
  the 48 bits of K4 (or as many as the active S-boxes reveal; brute-force the
  rest).

### 5. Reconstruct the full key and verify
- Invert the key schedule from `K4` to recover the 56-bit key (the remaining
  unknown key bits, if any, are found by exhaustive search over the small
  remaining space).
- Verify: encrypt fresh plaintexts with your recovered key offline and confirm
  the ciphertexts match the oracle.

## Deliverables

- A report containing:
  - The DDT analysis and the high-probability differentials you used.
  - The differential characteristic across the 4 rounds and its probability.
  - The step-by-step last-round subkey recovery.
  - The fully recovered key and verification results.
- The code used for the DDT analysis, pair generation, and key recovery.
