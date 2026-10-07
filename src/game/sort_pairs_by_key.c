/* sort_pairs_by_key: candidate for 1040:7560 (151 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Sorts two parallel byte arrays ascending by the second one, keeping the first in step.
 * It is a bubble sort that RESTARTS from the beginning after every swap: the inner loop
 * tests the swapped flag BEFORE the index, so one exchange ends the pass and the outer loop
 * begins again. That costs a pass per inversion, and it is why the flag is tested twice per
 * iteration in the shipped code.
 *
 * `int swapped = 1` with a `do`/`while` is what the entry sequence is: `MOV SI,1` followed
 * immediately by `XOR SI,SI` at the head of the first pass, with no jump to a test in
 * between. Written as `while (swapped)` Borland puts a `JMP` to the bottom test first, three
 * bytes earlier than the shipped code diverges.
 *
 * The keys are signed: the comparison is `JNG` after a byte `CMP`.
 *
 * THE DECLARATION IS THE WHOLE PUZZLE, and it extends rule 40. This function makes NO CALLS,
 * so Borland has the scratch registers to itself and there is no stack frame at all -- `SUB
 * SP` is absent, and four variables live in four registers: `n` in DI, `swapped` in SI, `j`
 * in DX and `t` in CL. Rule 40's SI/DI model does not reach that ; getting it needs
 *
 *     int swapped = 1;        // plain, and it still lands in SI
 *     register int j;         // DX
 *     register char t;        // CL
 *
 * and nothing else tried in sixteen spellings comes out. Marking `swapped` `register`
 * moves it to CX; leaving `t` plain spills it to the stack and brings `SUB SP,2` back;
 * making `t` an `int` moves the divergence 0x20 bytes later into the swap itself. A `char`
 * temporary that Borland is asked for by name is the only thing that reaches CL.
 */

#include "colonization.h"

void sort_pairs_by_key(int n, char *idx, char *keys)
{
    int swapped = 1;
    register int j;
    register char t;

    do {
        swapped = 0;
        for (j = 0; !swapped && j < n - 1; j++) {
            if (keys[j] > keys[j + 1]) {
                t = keys[j];
                keys[j] = keys[j + 1];
                keys[j + 1] = t;
                t = idx[j];
                idx[j] = idx[j + 1];
                idx[j + 1] = t;
                swapped = 1;
            }
        }
    } while (swapped);
}
