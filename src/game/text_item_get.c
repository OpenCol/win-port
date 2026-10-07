/* text_item_get: candidate for 1040:6efc (122 bytes, 9 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Copies the text cursor's next comma-separated field into the shared field buffer,
 * steps the cursor past the comma, trims both ends, and returns the buffer.
 *
 * RENAMED 2026-09-26 from next_csv_field: the Mac build's text_item_get(void), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between text_open and text_search, where the Mac gap holds 6 functions and the Windows one 7, so the pairing inside it is by size and arity (the Mac body is 0.93x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void fileio_purge_all_spaces(char *s);
char * text_item_get(void)
{
    char *d = g_field;

    while (*g_text_cursor != 0 && (int)*g_text_cursor != ',') {
        *d = *g_text_cursor;
        g_text_cursor++;
        d++;
    }
    if (*g_text_cursor != 0)
        g_text_cursor++;
    *d = 0;
    fileio_purge_all_spaces(g_field);
    return g_field;
}
