/* message_buf_clear: candidate for 1058:bead (25 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Empties the shared message buffer at DGROUP:0x4e6c -- one store of a zero byte to its first
 * character. It completes the set that `message_buf_set_1058_c0a6` (STRCPY) and
 * `message_buf_cat_1060_897c` (STRCAT) already name: set, append, clear.
 *
 * Eighteen call sites had it as `message_buf_clear`, because it takes no arguments and so says
 * nothing at the call. The shipped bytes say all of it.
 */

#include "colonization.h"

void message_buf_clear(void)
{
    g_message_buf[0] = 0;
}
