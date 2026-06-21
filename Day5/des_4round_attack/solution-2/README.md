# Solution 2: slide-based differential attack

Implements the **Figure 3 / slides 13–15** strategy from the DES cryptanalysis lecture:

| Characteristic | Plaintext XOR `P'` | Round-2 active S-boxes | Recovers |
|---|---|---|---|
| **Ω₁** | `0x2000000000000000` | S1 only | `K4` chunks for S2–S8 (42 bits) |
| **Ω₂** | `0x0222222200000000` | S2–S8 | `K4` chunk for S1 (6 bits) |

Then brute-force the S4 chunk (Ω₁ leaves S4 inactive in round 4), invert the key schedule, and verify against the oracle.

## Run the notebook

```bash
cd solution-2
pip install requests jupyter   # or use a venv
jupyter notebook cryptanalysis.ipynb
```

Or headless:

```bash
ORACLE_URL=http://des.rathanappana.com SLIDE_NUM_PAIRS=40 \
  jupyter nbconvert --execute --inplace cryptanalysis.ipynb
```

- **`SLIDE_NUM_PAIRS`** (default 40): pairs per characteristic → ~`4×NUM_PAIRS + 35` oracle queries.
- **`SKIP_ORACLE_PROBE=1`**: skip the 1-query connectivity check.
- Oracle limit: **1000 queries/day per IP**.

## Local dev script

```bash
SLIDE_NUM_PAIRS=40 python3 _dev_slide_attack.py   # uses ../.env key
```

Verified locally: recovers `K4 = 896E619BA7E3` and key `8B2F88A663CB4D`.

## Slide notation pitfall

The slides write ciphertext as `(l || r)` with **`l = R4`** (left half) and **`r = L4 = R3`**. Equation (8) is:

`D' = B' ⊕ l' = B' ⊕ R4'`  (not `B' ⊕ L4'`).

Round-4 F input difference is **`d' = r' = L4'`**.
