/* message_buf_set_1060_682a: candidate for 1060:682a (34 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `STRCPY(g_message_buf, s)` -- the shared message buffer at DGROUP:0x4e6c that
 * `message_buf_1030_e99a` and `message_buf_1058_bec6` both hand out. Its twin at 1060:684c is
 * the same function with `STRCAT` in place of `STRCPY`: copy and append to one buffer,
 * written as a pair.
 */

#include "colonization.h"

void message_buf_set_1060_682a(char *s)
{
    STRCPY(g_message_buf, s);
}
