# `shift_table_down` copies the wrong way, and the byte match cannot see it

**For the byte-exact repo.** Nothing over there has been changed; this is the evidence.

## What the shipped code does

`1028:019c` computes two addresses and hands them to Borland's far struct-copy helper:

```
mov  dx,0xe                       ; i = 14
...
imul ax,ax,0x6
add  ax,0x67c                     ; base + (i + 1) * 6
push SEG SEG24_base
push ax
imul ax,ax,0x6
add  ax,0x676                     ; base + i * 6
push SEG SEG24_base
push ax
mov  cx,0x6
call far F_SCOPY
dec  dx
```

Both operands are in the instruction stream; which one is the **destination** is not. The
write log settles it. Running the shipped function at `n = 0` in the emulator produces 45
word writes, all in `SEG24:0x67c .. 0x6d5` — that is `base + (i + 1) * 6`, for i = 14
down to 0. It reads the lower address and writes the higher one:

```c
g_table6[i + 1] = g_table6[i];      /* opens a gap at n */
```

## What the reconstruction says

`matched/game/shift_table_down.c` has `g_table6[i] = g_table6[i + 1];` — the other way
round, closing a gap rather than opening one. Its header comment says so too: *"Moves
entries n..14 of a table of 6-byte records down by one (entry i = entry i + 1)"*.

## Why the byte match passes anyway

`F_SCOPY` takes its two far pointers on the stack, and the compiler pushes both whichever
way the assignment is written — only the order of the two `add ax,imm` constants records
which is which. That order is decided by the helper's own convention rather than by the
assignment, so both spellings can compile to the same instruction sequence. The comparator
sees identical bytes and the program does the opposite thing.

## How it was found, and confirmed

`tests/differential.py` ran the ported function and the shipped bytes over n = 0..24 from
the same memory: 10 of 25 agreed, and the disagreements were always one element apart —
the port writing `base + i * 6` where the shipped wrote `base + (i + 1) * 6`. Reversing
the assignment makes it 25 of 25.

## Worth checking over there

This is the second reconstruction in this repo's first 350 ports whose bytes match and
whose behaviour does not, after `job_input_goods`. Both are cases where the *bytes* are
ambiguous about semantics — a jump table's entries, a helper's argument order — and the
comparator forgives exactly those. A sweep of every reconstruction that assigns one
struct to another, or switches on a jump table, would say whether there are more.
