/* message_buf_cat_1060_897c: candidate for 1060:897c (34 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `STRCAT(g_message_buf, s)` -- the shared message buffer at DGROUP:0x4e6c that
 * `message_buf_1030_e99a` and `message_buf_1058_bec6` both hand out. Its twin at 1058:c0a6 is
 * the same function with `STRCPY` in place of `STRCAT`: copy and append to one buffer,
 * written as a pair.
 *
 * TRANSFERRED: byte-identical to `message_buf_cat_1058_c084` (1058:c084) and reaching the same routine, one of
 * 7 copies of this function in the image.
 */

#include "colonization.h"

void message_buf_cat_1060_897c(char *s)
{
    STRCAT(g_message_buf, s);
}
