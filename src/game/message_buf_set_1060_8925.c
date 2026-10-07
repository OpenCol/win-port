/* message_buf_set_1060_8925: candidate for 1060:8925 (34 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `STRCPY(g_message_buf, s)` -- the shared message buffer at DGROUP:0x4e6c that
 * `message_buf_1030_e99a` and `message_buf_1058_bec6` both hand out. Its twin at 1058:c084 is
 * the same function with `STRCAT` in place of `STRCPY`: copy and append to one buffer,
 * written as a pair.
 *
 * TRANSFERRED from `message_buf_cat_1058_c084` (1058:c084), byte-identical except for the one relocated call:
 * this copy reaches `STRCPY` where that one reaches `STRCAT`. The comparator masks that
 * word, so the name comes from the disassembly and not from the verdict.
 */

#include "colonization.h"

void message_buf_set_1060_8925(char *s)
{
    STRCPY(g_message_buf, s);
}
