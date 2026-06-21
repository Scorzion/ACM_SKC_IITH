# ACM India Summer School on Symmetric Key Cryptography (IIT Hyderabad) 🔑🎓

Welcome to my archive of resources, slides, hands-on lab solutions, and technical reports from the **ACM India Summer School on Symmetric Key Cryptography**, hosted at **IIT Hyderabad (IITH)**. 

This repository contains my personal solutions to all coding labs (implemented in competitive programming style in C++ and Lean 4) alongside formal mathematical proofs and comprehensive LaTeX reports compiling my analysis for each day.

---

## 📅 Day-by-Day Overview

| Day | Core Topics | Sub-Topics & Hands-on Work | Solutions & Reports |
|:---:|:---|:---|:---:|
| **Day 1** | **Classical Cryptography** | Caesar, Shift, Atbash, Affine, Scytale, Rail Fence, Hill, and Vigenère ciphers. | [C++ Code & Report](file:///home/scorzion/Tech/Crypto/SKC_IITH/Day1/solutions/) |
| **Day 2** | **Modern Crypto Theory** | Indistinguishability proofs, Pseudo-random Generators (PRGs), and negligible functions. | [LaTeX Proofs](file:///home/scorzion/Tech/Crypto/SKC_IITH/Day2/solutions/) |
| **Day 3** | **Modern Block Ciphers** | Full implementations of DES and AES round functions, key schedules, and FIPS test vector verification. | [C++ Code & Report](file:///home/scorzion/Tech/Crypto/SKC_IITH/Day3/solutions/) |
| **Day 4** | **Linear Cryptanalysis** | Linear S-box approximation, computing linear approximation tables, and linear key-recovery attacks (Heys Cipher/DES). | [C++ Code & Report](file:///home/scorzion/Tech/Crypto/SKC_IITH/Day4/solutions/) |
| **Day 5** | **Differential Cryptanalysis** | Difference Distribution Tables (DDT) and performing active 4-round differential cryptanalysis attacks on DES. | [C++ Code & S-Box Attack](file:///home/scorzion/Tech/Crypto/SKC_IITH/Day5/solutions/) |
| **Day 6** | **Stream Ciphers** | Salsa20 core architecture, state matrices, column/row round transformations, and keystream generation. | [C++ Code & Report](file:///home/scorzion/Tech/Crypto/SKC_IITH/Day6/solutions/) |
| **Day 7** | **Hardware Security** | Architectural leakage, cache timing attacks, and hardware threat modeling with the ChampSim simulator. | [Lab Guidelines](file:///home/scorzion/Tech/Crypto/SKC_IITH/Day7/) |
| **Day 8** | **Machine Learning & ML-based Distinguishers** | Speck32/64 cipher implementation, Gohr's differential path analysis, and generating $10^7$ pair differential simulations. | [C++ Code & Report](file:///home/scorzion/Tech/Crypto/SKC_IITH/Day8/solutions/) |
| **Day 9** | **Formal Verification & Theorem Proving** | Mathematical verification using **Lean 4**: inductive parity predicates, partial-order minimums, list operations, sorting correctness, and permutation invariance proofs. | [Lean 4 Proofs & Report](file:///home/scorzion/Tech/Crypto/SKC_IITH/Day9/solutions/) |
| **Day 10** | **Post-Quantum Crypto & Privacy** | TCS Research industry session on PQC Migration/Risk Assessment, and Fully Homomorphic Encryption (FHE) for PPML. | [Materials](file:///home/scorzion/Tech/Crypto/SKC_IITH/Day10/) |

---

## 🛠️ Repository Structure
Every `DayX/` directory is self-contained and typically structured as follows:
*   `problems/` or `*.pdf` / `*.txt`: The official handouts, templates, and lecture slides.
*   `solutions/`:
    *   Compact, optimized C++ implementations (CP-style, commentless as required by the labs).
    *   Lean 4 proof files (`.lean`).
    *   LaTeX source files (`report.tex`) and pre-compiled report PDFs (`report.pdf`).

---

## 💡 Highlight Highlights & Special Thanks

*   🪃 **The Boomerang Attack**: A special shoutout to **Prof. Dhiman Saha** (IIT Bhilai) for his masterclass on Boomerang attacks. His intuitive and rigorous explanation of differential propagation and boomerang mechanics was one of the school's absolute highlights.
*   💻 **Formal Verification**: Exceptional session by **Dr. Ashish Mishra** on **Lean 4**, bridging the gap between abstract mathematical proofs and concrete program security verification.

Many thanks to the outstanding organizing committee at IIT Hyderabad for hosting this incredible school:
*   **Prof. M. V. Panduranga Rao**
*   **Prof. Maria Francis**
*   **Dr. Nitin Kumar Sharma**
*   **Ms. Thakkalapally Preethi**

Special thanks to the **Center for Cryptography and Cybersecurity (CCS)** and **ACM India** for putting this event together, and to **Tata Consultancy Services (TCS) - Research** for sponsoring the school.
---

## 👤 Author
*   **Sourav Sharma** - *M.Sc. Student, IISER Bhopal* (email: [souravs24@iiserb.ac.in](mailto:souravs24@iiserb.ac.in))
