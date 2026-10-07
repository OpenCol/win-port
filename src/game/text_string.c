/* text_string: candidate for 1040:6fbd (51 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: text_string
 *
 * Advances to the next text line and parses the buffer at SEG29:0x3c with
 * strings_store.  text_get is -called and defined in-file as a
 * stub.
 *
 * RENAMED 2026-09-26 from read_next_text_value: the Mac build's text_string(void), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between text_open and text_search, where the Mac gap holds 6 functions and the Windows one 7, so the pairing inside it is by size and arity (the Mac body is 0.98x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order. The Mac text_string is text_get then strings_store, as this is text_get then text_store_string.
 */

#include "colonization.h"

extern int g_stub_a;
extern int strings_store(char *s);

extern void text_get(void);

int text_string(void)
{
    int r;

    text_get();
    r = strings_store(g_text_buf);
    return r;
}
