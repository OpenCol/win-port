/* text_item_binary: candidate for 1040:7023 (100 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Reads the next CSV field as a binary number of '0' and '1' characters into a byte.
 *
 * RENAMED 2026-09-26 from parse_binary_field: the Mac build's text_item_binary(void), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between text_open and text_search, where the Mac gap holds 6 functions and the Windows one 7, so the pairing inside it is by size and arity (the Mac body is 0.86x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern char *text_item_get(void);
unsigned char text_item_binary(void)
{
    unsigned char v = 0;
    char *p;

    text_item_get();
    p = g_field;
    while ((int)*p == '0' || (int)*p == '1') {
        v *= 2;
        if ((int)*p == '1')
            v++;
        p++;
    }
    return v;
}
