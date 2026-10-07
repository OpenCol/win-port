/* memory_is_low: candidate for 1070:d22d (117 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The game's own out-of-memory predicate, and the reason the timer at `LowMemTimerProc` runs
 * at all: true when free global memory is under `kb` kilobytes, or when ANY of the three USER
 * resource heaps (GDI=1, USER=2, combined=0) is under 15 percent free. Its one caller passes
 * 1500, so the warning starts at 1.5 MB.
 *
 * Four independent `if`s all assigning the same 1, not a chain of `||`: the shipped code calls
 * all four APIs unconditionally and every test jumps forward over the same `MOV SI,1`.
 *
 * `low` is a `register int` -- SI, zeroed by the prologue's `XOR SI,SI` before the first call.
 *
 * The GetFreeSpace comparison is UNSIGNED (`JNC`, not `JL`), which is what makes `need`
 * `unsigned long`; `kb` is still a signed `int`, so the conversion sign-extends (`MOVSX`)
 * before the shift.  Rule 48.
 *
 * 117 bytes, not the census 109: the written-out `return` adds a dead second epilogue
 * (rule 44).
 */

#include "colonization.h"

int memory_is_low(int kb)
{
    register int low = 0;
    unsigned long need = (unsigned long)kb << 10;

    if (GetFreeSpace(0) < need)
        low = 1;
    if (GetFreeSystemResources(1) < 15)
        low = 1;
    if (GetFreeSystemResources(2) < 15)
        low = 1;
    if (GetFreeSystemResources(0) < 15)
        low = 1;
    return low;
}
