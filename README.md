## Addendum

### Day 4

The complete process to use profile-guided optimization (PGO) with Clang is as follows:
- Know that `-fprofile-instr-generate` instructs the compiler to instrument code at the source instruction level, while `-fprofile-generate` instructs the compiler to instrument code at the LLVM intermediate representation (IR) level.
- We want to generate the profile first by running:
```bash
clang -O3 -march=native -funroll-loops -flto -fprofile-instr-generate d4.c parse_input.c -o d4
```
- We then want to actually *run the program*:
```bash
./d4
```
- Clang will generate a `default.profraw` file for you. Merge it:
```bash
llvm-profdata merge -output=default.profdata default.profraw
```
- Then you can compile again with the profile:
```bash
clang -O3 -march=native -funroll-loops -flto -fprofile-instr-use d4.c parse_input.c -o d4
```
- Voila!