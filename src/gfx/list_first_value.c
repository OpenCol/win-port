/* list_first_value: candidate for 1068:9e24 (73 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 73, not the census 67. The last 6 bytes are the `JMP` past the `else` and
 * the unreachable epilogue it lands on.
 *
 * Rewinds the cursor to the head and returns the head's value. The iterator record holds
 * the head at +4 and the cursor at +8, both pointers; `VList_Next` advances the cursor
 * and `list_current_value` reads it without moving.
 *
 * An empty list returns 0 through the second of the THREE epilogues. The third is the
 * fall-through, and it is there only because the source spells the alternative as an
 * explicit `else` -- `if (...) return x; return 0;` gives two epilogues and no `JMP`.
 */

#include "colonization.h"

#include "records.h"
/* same object: 4-byte `push cs; call ` */

long list_first_value(ListIter *it)
{
    if (it->head != 0) {
        it->cur = it->head;
        return deref_long(it->head);
    } else {
        return 0L;
    }
}
