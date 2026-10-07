/* PORT: this file's own declarations of heap_get removed: the header declares it from the definition (W1 step 3). */
/* strings_store: candidate for 1040:76f0 (94 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: strings_store
 *
 * Copies a string into the text pool at SEG29:0x10dc (heap_get
 * hands out the space) and returns the running string count at
 * DGROUP:0x1cf4, which it increments.  The allocator is -called and
 * defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from text_store_string: the Mac build's strings_store(char*), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits in the same uneven gap as its siblings (the Mac body is 1.02x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order. Called by the two text_*_string readers on both sides, after the read.
 */

#include "colonization.h"

#include "records.h"

extern char g_stub_s[];


int strings_store(char *s)
{
    int n;
    char *p;

    n = STRLEN(s) + 1;
    p = (char *)heap_get(&g_text_pool, n);
    STRCPY(p, s);
    return g_text_count++;
}
