# Why 78 C++ methods are not verified, and what has been ruled out

**For win-port, and for anyone who picks this up.** 53 of 131 C++ methods verify in 16
bits. This is what the other 78 are blocked on, with the measurements.

## `this` is on the stack — measured, not assumed

`CDControlWin::~CDControlWin()` at `1048:e49a`, run four ways in the emulator with a
staged region at `SEG34:0x2b00`:

| how `this` was passed | returned | instructions | imports called |
| --- | --- | --- | --- |
| **on the stack, `[offset, selector]`** | no | 141 | **`USER.72`** |
| in DX:AX | yes | 17 | none |
| in SI, DS set | yes | 17 | none |
| DS:SI with offset on the stack | no | 141 | `USER.72` |

The stack form is what makes the function do its work. That is what
`tools/port.py` already generates, so the calling convention was never the problem.

## The candidate and the shipped function differ eight bytes in

Both start with the same `-WS` prologue and then part:

```
shipped:   8c d0 90 45 55 8b ec 1e  8e d8 56 8b 76 0a ...   MOV DS,AX; PUSH SI; MOV SI,[BP+0A]
candidate: 8c d0 90 45 55 8b ec 1e  00 9a ff ff 76 0a ...   a far call where DS is loaded
```

So the compiled port is not the same code as the shipped function at that point. This is a
CODEGEN difference, not a fixup one: the candidate really does have a call there.

## What has been ruled out

- the calling convention (measured above);
- the fixup offsets. TDUMP reports a FIXUPP offset relative to the LEDATA record before it
  and `tests/emu_differential.py` now carries that base. It was wrong before and is
  harmless for a single-record C object, but correcting it did not change these bytes.

## What to look at next

The `-WS` (smart callback) prologue and how Borland emits DS loading for a member function
of a class with a virtual table. `docs/findings/borland-codegen.md` in the byte-exact repo
is the place that would know; that repo matched these functions byte for byte, so its
build flags for a C++ unit are the answer. Compare them against `FLAGS` in
`tests/emu_differential.py` (`-ml -WS -Ot -3 -dc -DCOL_EMU`).

Two of the 78 are a different, harder thing: a method that reaches a callee with a NEAR
call to an external. No harness that loads the candidate in a selector of its own can run
those, exactly as with a rule-47 forwarder.
