/* message_buf_cat_1060_684c: candidate for 1060:684c (34 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `STRCAT(g_message_buf, s)` -- the shared message buffer at DGROUP:0x4e6c that
 * `message_buf_1030_e99a` and `message_buf_1058_bec6` both hand out. Its twin at 1060:682a is
 * the same function with `STRCPY` in place of `STRCAT`: copy and append to one buffer,
 * written as a pair.
 */

#include "colonization.h"

void message_buf_cat_1060_684c(char *s)
{
    STRCAT(g_message_buf, s);
}
