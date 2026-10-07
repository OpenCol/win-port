/* PORT: this file's own declarations of text_item_get removed: the header declares it from the definition (W1 step 3). */
/* text_item_string: candidate for 1040:6ff0 (51 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: text_item_string
 *
 * Advances to the next comma-separated field and parses the field buffer
 * at SEG29:0x8c with strings_store.  text_item_get is -called and
 * defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from read_next_csv_value: the Mac build's text_item_string(void), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between text_open and text_search, where the Mac gap holds 6 functions and the Windows one 7, so the pairing inside it is by size and arity (the Mac body is 1.10x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order. The Mac text_item_string is text_item_get then strings_store, as this is next_csv_field then text_store_string; its twin text_string is text_get then strings_store, which is read_next_text_value.
 */

#include "colonization.h"

extern int g_stub_a;
extern int strings_store(char *s);


int text_item_string(void)
{
    int r;

    text_item_get();
    r = strings_store(g_field);
    return r;
}
