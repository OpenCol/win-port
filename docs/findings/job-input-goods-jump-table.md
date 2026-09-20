# `job_input_goods` reads the wrong goods for two jobs, and the byte match cannot see it

**For the byte-exact repo.** Nothing here has been changed over there; this is the
evidence, so someone who owns that tree can decide what to do with it.

## What the shipped code does

`1018:4b88` dispatches on `job` through a CS-relative jump table:

```
mov  bx,[bp+0xa]        ; job
sub  bx,0x13
cmp  bx,0x5
ja   short L4_4bfb      ; default: write nothing
add  bx,bx
jmp  word near [cs:bx+0x4c05]
```

The table at `SEG4:0x4c05`, and the arm each entry reaches:

| job | entry | arm | writes |
| --- | --- | --- | --- |
| 0x13 | 0x4bf9 | default | nothing |
| 0x14 | 0x4bb7 | 0x4bba | **0xe** |
| 0x15 | 0x4ba6 | 0x4ba9 | **0xf** |
| 0x16 | 0x4bc8 | 0x4bcb | 8 |
| 0x17 | 0x4bd9 | 0x4bdc | 0xf, then 8 |
| 0x18 | 0x4bf9 | default | nothing |

## What the reconstruction says

`matched/game/job_input_goods.c` has `case 0x14: goods[n] = 0xf;` and
`case 0x15: goods[n] = 0xe;` — the two jobs the other way round. The curated description
in `re/symbols.json` repeats it: *"job 0x14 takes goods 0xf, 0x15 goods 0xe"*.

## Why the byte match passes anyway

The **arms** are identical either way: Borland emits them in source order, and the
shipped arm order is 0xf first, 0xe second, which is what both spellings produce. Only
the **table** differs — entries 1 and 2 are exchanged — and those twelve bytes are
address-shaped, so the comparator forgives them the way it forgives a relocation.

For the bytes to match exactly, the original source must have had the cases in the order
`0x15` then `0x14`, with `0x15` writing 0xf. The reconstruction sorted them and swapped
the constants to keep the arms in place, which preserves every byte the comparator looks
at and inverts the behaviour for two jobs.

## How it was found

`tests/differential.py` in win-port ran the ported function and the shipped bytes over
jobs -4..24 from the same staged memory. 27 of 29 agreed; the two that did not were
`job=0x14` (shipped wrote 0xe, the port wrote 0xf) and `job=0x15` (the reverse). No other
argument disagreed, which is what pointed at the table rather than at the arms.

## What win-port did

`src/game/job_input_goods.c` is hand-written from the table above and marked as not
regenerable, because `tools/port.py` would reproduce the swap. It is the only
hand-corrected port in the repo.
