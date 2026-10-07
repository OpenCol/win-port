/* text_item_number: candidate for 1040:6f76 (31 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: text_item_number
 *
 * The next comma-separated field parsed as a number. Both halves are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from parse_next_number: the Mac build's text_item_number(void), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits in the same uneven gap as its siblings (the Mac body is 1.42x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

extern char g_stub_s[];
extern int g_stub_i;

extern char * text_item_get(void);
extern int stoi(char *s);

int text_item_number(void)
{
    return stoi(text_item_get());
}
