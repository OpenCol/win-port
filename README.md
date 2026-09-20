# win-port — a Colonization that behaves like the shipped one and compiles

This repo builds a playable *COLONIZE.EXE* from the 3,173 functions reconstructed in
[col-win-re](../../dderevjanik/col-win-re). That repo's contract is **byte-exact**: every
reconstruction compiles to bytes identical to a function inside the shipped binary. This
repo's contract is weaker and different:

> **The program must behave identically. It need not produce the same bytes.**

Nothing here may be taken as evidence about the shipped binary. The byte-exact repo is the
authority on what the game does; this repo is where that knowledge is turned back into a
program. Its `game/` directory is evidence and immutable, and nothing in this tree writes
to it or to anything else over there — the Borland toolchain is symlinked in, read-only.

## The toolchain, and why

**Borland C++ 4.52, 16-bit, `-ml`, targeting Windows 3.1, under DOSBox-X.** The same
compiler that built the game. Not a 32/64-bit port.

That was a real fork and it was decided on two facts:

1. **The oracles are 16-bit.** The acceptance test for "behaves identically" is
   `tools/playtest_diff.py` in the byte-exact repo: it boots both binaries in a software
   Win16 emulator and compares the Win16 API call sequence, the call sites as raw
   `seg:off`, the WinG framebuffer, and the windows created. Three of those four surfaces
   do not exist in a 32-bit build. The per-function differential is equally 16-bit — it
   compiles the candidate with Borland and runs it inside the emulated machine. Choosing a
   modern target means deleting both oracles on day one and rebuilding them before any
   code may be moved. The one host-native layer has two test cases.
2. **`int` is 16 bits in about 28,000 places.** The map code wraps at 16 bits, and the
   width and signedness of a compare is what decides `JL` from `JB` — the thing the
   byte-exact repo spent its width-conflict work getting right. A modern target requires
   an `int` → `int16_t` sweep across the whole program, done with no differential running.

A 32/64-bit port is a later project that should start from a green whole-binary
differential, not replace it.

### One correction to the brief this repo was written from

"far/near is irrelevant to you — delete it" is right for **access** and wrong for
**placement**. Both spellings reach the same memory, so no site needs `far` to read a
global. But DGROUP is 64 KiB and must also hold the heap and the stack, while the model's
data totals about 88 KiB — segment 20 alone runs to offset 0xded4 (57,044). So a global is
*defined* `far` exactly when the shipped program kept it outside DGROUP. That is read from
`model.json`'s segment field, not chosen. See the header of `tools/gen_model_header.py`.

## Layout

| path | what |
| --- | --- |
| `model/model.json` | the handoff artefact, copied at `ab39b62`; `provenance.json` has its sha256 |
| `tools/gen_model_header.py` | generates the shared header from the model |
| `include/col/records.h`, `globals.h` | **generated** — do not edit; regenerate |
| `include/col/types.h`, `win16.h`, `colmem.h` | hand-written: widths, the platform seam, the host's memory |
| `include/*.h` | the `#define`-only headers, ported unchanged from the byte-exact repo |
| `tools/extract_segments.py` | lifts the shipped data segments out of `COLONIZE.EXE` |
| `src/segdata.c`, `include/col/segments.h` | **generated** — the 15 data segments, 123,164 bytes, byte for byte |
| `model/relocations.json` | the 193 far pointers the Windows loader used to fill in |
| `src/game/*.c` | ported functions |
| `tools/port.py` | moves one reconstruction into this repo and writes its case, or refuses and says why |
| `tools/bcc.py` | compiles units with the real Borland C++ 4.52 under DOSBox-X |
| `tests/differential.py` | the harness: a ported function against the shipped bytes |
| `tests/verify_segments.py` | is `segdata.c` really the shipped image? an independent loader answers |
| `tools/check.sh` | all of the above |

`include/README.md`'s prohibition on a shared header is a property of the byte-exact
contract and does not apply here. The `#define`-only headers it describes port unchanged.

## What exists, and what it has been checked against

- **The generated header compiles under the real Borland C++ 4.52 targeting Win16.**
  1,161 globals over 1,034 objects, 170 records. Every record carries a compile-time size
  assertion, and Borland accepts all of them — which is a check that the model's offsets
  and the 16-bit byte-aligned layout agree, not a formality.
- **The program's data is the shipped data.** All 15 DATA segments are lifted out of
  `COLONIZE.EXE` and every global is a macro into one of them at its shipped offset.
  `tests/verify_segments.py` compares all 123,164 bytes against the same image loaded by
  an independent loader — the byte-exact repo's emulator, which walks the relocation
  chains, none of which the extractor does — and finds **0 unexplained differences**, with
  every one of the 193 relocation sites accounted for.
- **`src/segdata.c` compiles under Borland too**, to a 127 KB object.
- **The header compiles host-native too**, and both targets address memory identically:
  the segment image is the storage on each. That is what lets an observation from the
  emulator and one from the host be compared as the same coordinate, and it is why the
  differential can now run against `--state shipped` rather than only a save.
- **128 functions are ported and verified**: 5,405 argument sets, 0 disagreements, on
  return value and on memory afterwards, against `--state shipped`. All 128 also compile
  with the real Borland C++ 4.52 targeting Win16.
- **The harness's own null case holds**: `--selftest` breaks every port on purpose and all
  256 mutations are caught. A mutation that changes no source text is reported rather than
  counted, and a case that compares no return value and observes no write is reported as
  verifying nothing rather than passing.

```
tools/check.sh          # regenerate-and-compare, selftest, differential, Borland compile
```

## Porting

`tools/port.py` moves a reconstruction here and writes its case. A reconstruction is a
standalone translation unit — it re-declares every type and global because a byte-exact
file cannot include a shared header — so the port is mostly deletion, and deletion is
where a port quietly changes meaning. It therefore refuses rather than guesses, and a
refusal names its reason. Of 3,173 functions, 130 are portable as they stand; the rest
wait on tier B (1,617 declare their own struct, whose members must be mapped onto the
merged record by offset), on their callees, or on a limitation below.

Three functions were ported, verified to compare nothing, and withdrawn: the generated
sweep never reached the branch that writes, so the case was green without testing
anything. Nothing sits in `src/` that the harness has not checked.

### What the harness cannot see

Stated so a pass is not read as more than it is. The host side has no MMU; it diffs
memory. So it does not see **reads** or write **order**. It says nothing about calling
convention, stack discipline or segment arithmetic — those belong to the whole-binary
differential, which needs a linked `.EXE` and is not stood up yet. A function whose
*result* depends on a wrong read still fails here.

A write of a value that was already there used to be invisible too, which let a port that
did nothing at all pass on seven buffer-clearing functions. The shipped side now runs each
argument set twice — once to learn which bytes it writes, then again with exactly those
bytes poisoned to the complement of what they held — and the host is given the same
poison, so every write changes something on both sides.

Three things cannot be compared on a host at all, and `port.py` refuses them: a **pointer-
valued global** (four bytes in the game, eight on the host, so its stored value there is an
encoding and following it faults), a **`long` argument** (the sweep hands the emulator one
16-bit word per argument), and a function that **returns an uninitialised local** — which
one stubbed-out routine left in the shipped build does, making its answer whatever the
registers held.

## What is not done

Ordered by how much it can hurt.

1. **100 of the 193 relocations cannot be written yet.** These are far pointers the
   Windows loader filled in; the file holds a chain offset instead. 93 point into data and
   `col_relocate()` sets them at startup. The other 100 point at **code** — function
   pointers in dispatch tables — and cannot be written until those functions are ported.
   `model/relocations.json` lists each with the name it wants, from the symbol map. Until
   then a dispatch through one of those tables goes nowhere, so they are the natural
   companion to porting each function.
2. **No link.** No `.DEF`, no resource binding, no `.EXE`. The three inputs are known and
   `tools/rebuild.py` in the byte-exact repo generates all three from the shipped header
   (heap, stack, AUTODATA, 180 IMPORTS) and binds 42 resources with BRC.
3. **No translation units yet.** 3,173 bodies still to move, each needing its per-file
   typedefs and externs deleted and its rule-47 in-file stubs (~3,685 definitions)
   removed; `check_call_forms.py` over there identifies them. The partition is partly
   recoverable: 6,789 four-byte near-call sites give 104 forced units (largest ~260
   functions) plus 907 unconstrained. A flat build also works once the stubs are gone.
4. **`reconstructed/unresolved/colonist_take_job.c` is knowingly unmatched** over there and
   must not be ported as if it were settled.

## Inherited unfinished business

From the model, unresolved and carried forward rather than papered over:

- **143 width conflicts and 127 member-name conflicts** in `model.json`. Where two
  spellings of one address differ in width the generator emits the **wider**, so no site
  can over-read, and records the narrow views — express those as a cast at the site
  (`*(unsigned char *)&rec.field`), never as a second declaration.
- **43 globals where two spellings differ only in signedness at equal width.** Counting
  files cannot settle these and has been wrong every time it was checked. The generator
  emits one, marks it, and `model/derived.json → signedness_unresolved` names every
  address. The shipped instruction decides: `tools/re_kb.py context <addr> --asm`.
- **29 member names that mean two offsets inside one record.** Renamed to `name_<offset>`
  to compile; `model/derived.json → member_name_collisions` lists them. A rename is not a
  judgement about which is right.
- **6 records are merged buckets, not types**: `Rec` (1,916 bytes, one field named `v`
  three times), `Owner`, `Obj`, `Win`, `ColonyWin`, `Entry`. Every file that called its
  anonymous struct `Rec` was unioned into one definition. The generator refuses to pick a
  bucket as a pointee when a well-formed record competes — that alone turned
  `g_cur_colony_rec` from `Rec *` back into `Colony *` — but the buckets themselves are
  still wrong and will mislead anyone who reads them as layouts.
- **360 globals have no declared extent** (`char far @[]`). The generator infers each from
  the gap to the next global in the same segment, which is an **upper bound**, and marks
  it. Since the storage is the whole segment image rather than 1,034 separate objects,
  a wrong guess is largely harmless: the bytes past the guess are the shipped bytes and in
  the shipped order, so code that walks past the end reads what it read in 1995. The
  extents still matter for anything that takes `sizeof`, which is why they are listed.
- **127 names are views into another object**, 101 at the same address and 26 inside
  another's extent. They are emitted as macros, not as second declarations, so a write
  through either name is seen by the other. `SEG31:0x00a4` carries eight names on purpose
  (an array of edit buffers and buffer 0) and comes out as one object — that is correct,
  not a defect.
- **72 offsets where two meaningful names compete** (`BuildingRec+0x03`: `prereq` or
  `prev_level`), and **24 globals whose address is underivable** — reached only through
  far-pointer constants where the loader relocates the offset and the bytes hold `ff ff`.
- **Four records are Windows' own.** `MSG`, `RGBQUAD` and `TEXTMETRIC` match the platform
  and the platform's win; their member names differ from the model's and each porting site
  adapts. `BITMAPINFO` does **not** match — the game's is 1,064 bytes (a header plus a full
  256-entry colour table) against windows.h's 44 — so it is kept as `ColBITMAPINFO`. Using
  the platform's there would overflow a real buffer.

## The one rule that settled every dispute

On any disagreement about a name, a width or a meaning: **find the function that writes the
value and take its reading.** That beat the majority six times out of six. Counting files
was wrong every time it was checked. And prefer the shipped binary over a comment —
several comments have been wrong; the bytes never are.
