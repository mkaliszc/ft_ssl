# ft_ssl — md5 & sha256

A from-scratch reimplementation of part of OpenSSL in C: the `md5` and `sha256`
message-digest commands. This is the first project of the ft_ssl branch, so the
binary is built to be extended by later commands (`base64`, `des`, `rsa`, …)
without rewriting what already exists.

## Usage

```
make
./ft_ssl command [flags] [file/string]
```

| Flag | Effect |
|---|---|
| `-p` | echo STDIN to STDOUT, then print its digest |
| `-q` | quiet mode: print the digest only |
| `-r` | reverse format: `digest name` instead of `MD5 (name) = digest` |
| `-s <string>` | hash the given string |

```
> echo "42 is nice" | ./ft_ssl md5
(stdin)= 35f1d6de0302e2086a4e472266efb3a9
> ./ft_ssl sha256 -s "42 is nice"
SHA256 ("42 is nice") = b7e44c7a40c5f80139f0a50f3650fb2bd8d00b0d24667c4c2ca32c88e13b758f
> ./ft_ssl md5 -r file
53d53ea94217b259c11a5a2d104ec58a file
```

Parsing rules:
- `-s` consumes exactly one argument.
- The first argument that is not a flag ends flag parsing: everything after it is a file.
- STDIN is read when `-p` is set **or** when no other source was given, and it is always processed first.
- A missing or unreadable file prints an error and the remaining sources are still processed.

Only `open close read write malloc free` are used, plus `strerror` and `exit`.

---

## Architecture

### Execution flow

```
main
 └─ dispatch_cmd                 table of { name, run } — no if/else chain on command names
     └─ md5_func / sha256_func   pick the algorithm spec, then:
         ├─ digest_parser        flags (bitmask) + ordered list of sources
         ├─ digest_exec          for each source:
         │    ├─ hash_operand        init → update … → final
         │    │    └─ feed_fd        fixed 4 KiB read loop (files and STDIN)
         │    └─ print_result        one of the 4 output shapes
         └─ ssl_digest_cleaner   frees everything
```

### Key design choices

- **One algorithm = one spec.** Each algorithm exposes a `t_digest_spec`
  (label, digest size, `init` / `update` / `final` pointers). The execution
  loop, the reading and the display are written once and never know which
  algorithm they run. Adding a hash means writing a spec, not new plumbing.
- **Streaming.** Sources are read with a fixed-size buffer and fed to `update`.
  There is no maximum input size and a file is never loaded in memory.
- **Shared core.** Buffering and padding are identical for MD5 and SHA-256, so
  they live in `digest_core.c`. Each algorithm only provides its block
  transform and the byte order of the length field, passed as function pointers.
- **No pointer casts on data.** 32/64-bit words are built byte by byte
  (`load32_le/be`, `store32_le/be`, `store64_le/be`), so the result does not
  depend on the host endianness or on memory alignment.
- **`-p` without `realloc`.** STDIN content is kept as a linked list of chunks
  (one per `read`), only when `-p` is set, to be echoed after hashing.

### Worktree

```
.
├── Makefile                 finds every src/**/*.c, clones/builds OctoLIB
├── include/
│   └── ft_ssl.h             types, constants, prototypes (single header)
├── OctoLIB/                 personal libft (ft_printf, printf_fd, mem/str utils)
└── src/
    ├── main.c               usage when no command is given
    ├── dispatch.c           command table
    ├── common/              byte helpers reusable by the next ft_ssl commands
    │   ├── endian.c         load/store 32 and 64-bit words, LE and BE
    │   └── hex.c            raw digest -> lowercase hex string
    └── digest/              everything specific to the hash family
        ├── parsing.c        flags and sources
        ├── exec.c           per-source loop, file/STDIN reading
        ├── digest_core.c    shared buffering (update) and padding (final)
        ├── md5/
        │   ├── run.c        md5 command entry point + init/update/final + spec
        │   └── md5_transform.c    64-round compression of one 64-byte block
        ├── sha256/
        │   ├── run.c        sha256 command entry point + init/update/final + spec
        │   └── sha256_transform.c 64-round compression of one 64-byte block
        └── utils/
            ├── operand.c    source list (STDIN pushed front, others appended)
            ├── chunk.c      STDIN chunk list for -p
            ├── display.c    output formats (-p -q -r, stdin / string / file)
            └── cleaner.c    all memory release
```

---

## How the algorithms work

Both MD5 and SHA-256 follow the **Merkle–Damgård construction**: the message is
cut into 64-byte blocks, and a fixed-size internal state is updated block after
block by a *compression function*. The final state is the digest.

```
IV ──▶ [C] ──▶ [C] ──▶ [C] ──▶ … ──▶ [C] ──▶ digest
        ▲       ▲       ▲             ▲
     block 1 block 2 block 3     last block (padding + length)
```

### Common steps

1. **init** — load the initial state (IV) and reset the counters.
2. **update** — absorb data of any length:
   - complete the block left pending by the previous call, and compress it;
   - compress every full 64-byte block directly from the input;
   - keep the remaining bytes (< 64) for the next call.
3. **final** — pad the message, compress the last block(s), write the state out:

```
[ remaining data ][ 0x80 ][ 0x00 … 0x00 ][ message length in bits, 8 bytes ]
                                          └── byte 56 to 63 of the last block
```

If fewer than 9 bytes are left in the block, an extra block is added.
Including the length prevents two different messages from giving the same
padded input.

### MD5

| | |
|---|---|
| Digest | 128 bits (16 bytes) |
| State | 4 × 32-bit words `a b c d` |
| Byte order | little-endian (message words, length, output) |
| IV | `0x67452301 0xefcdab89 0x98badcfe 0x10325476` |

Compression of one block:

1. Read the block as 16 little-endian words `M[0..15]`.
2. 64 steps in 4 rounds of 16. Each round has its own boolean function and
   its own order for picking message words:

   | Steps | Function | Word index |
   |---|---|---|
   | 0–15  | `F = (b & c) \| (~b & d)` | `i` |
   | 16–31 | `G = (d & b) \| (~d & c)` | `(5i + 1) mod 16` |
   | 32–47 | `H = b ^ c ^ d` | `(3i + 5) mod 16` |
   | 48–63 | `I = c ^ (b \| ~d)` | `7i mod 16` |

3. Each step: `b = b + rotl(a + f + K[i] + M[g], s[i])`, then the registers
   rotate (`a ← d ← c ← b`).
4. **Feed-forward**: the result is added to the incoming state.

`K[i] = floor(|sin(i + 1)| × 2³²)` and `s[i]` are fixed rotation amounts.

**Security:** broken. Practical collisions exist since 2004 (Wang et al.) and
can be found in seconds. The 128-bit output also caps generic collision
resistance at 2⁶⁴. MD5 must not be used for signatures or integrity against an
attacker.

### SHA-256

| | |
|---|---|
| Digest | 256 bits (32 bytes) |
| State | 8 × 32-bit words `a b c d e f g h` |
| Byte order | big-endian (message words, length, output) |
| IV | fractional parts of √ of the first 8 primes |

Compression of one block:

1. **Message schedule** — the 16 big-endian words are expanded to 64:
   ```
   W[i] = σ1(W[i-2]) + W[i-7] + σ0(W[i-15]) + W[i-16]
   σ0(x) = rotr(x,7)  ^ rotr(x,18) ^ (x >> 3)
   σ1(x) = rotr(x,17) ^ rotr(x,19) ^ (x >> 10)
   ```
2. **64 rounds**:
   ```
   T1 = h + Σ1(e) + Ch(e,f,g) + K[i] + W[i]
   T2 = Σ0(a) + Maj(a,b,c)
   h←g, g←f, f←e, e←d+T1, d←c, c←b, b←a, a←T1+T2

   Ch(e,f,g)  = (e & f) ^ (~e & g)          each bit of e chooses f or g
   Maj(a,b,c) = (a & b) ^ (a & c) ^ (b & c) majority vote per bit
   Σ0(a) = rotr(a,2) ^ rotr(a,13) ^ rotr(a,22)
   Σ1(e) = rotr(e,6) ^ rotr(e,11) ^ rotr(e,25)
   ```
3. **Feed-forward**: the result is added to the incoming state.

`K[i]` = fractional parts of ∛ of the first 64 primes.

**Why it is still secure while MD5 is not:**
- the message schedule *derives* new words instead of reusing the same 16
  in a new order, so a local difference spreads quickly and cannot be steered;
- two registers are updated per round, with two boolean functions;
- 256-bit output: 2¹²⁸ for a generic collision, 2²⁵⁶ for a preimage;
- the best known practical collision covers 31 of the 64 rounds, leaving a
  large security margin.

Limitation: like every Merkle–Damgård hash, SHA-256 allows **length
extension**. `SHA256(secret ‖ message)` is not a safe MAC; use HMAC instead.

### Why the constants look like that

`K[i]`, the IVs and MD5's rotation amounts are *nothing-up-my-sleeve numbers*:
derived from sin, square roots and cube roots so that anyone can recompute them
and nobody could have picked weak values on purpose. They also make every round
different, which breaks the symmetry an attacker could exploit.


